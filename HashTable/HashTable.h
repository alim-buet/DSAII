#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <vector>
#include <string>
#include "RedBlackTree.h"
#include "Utils.h"

using namespace std;

class HashTable
{
public:
    int maxSize, currentSize;
    virtual ~HashTable() {}

    double getLoadFactor()
    {
        return (double)currentSize / maxSize;
    }

    virtual int insert(const string &key, int value) = 0;
    virtual void remove(const string &key) = 0;
    virtual int search(const string &key) = 0;
    virtual void clear() = 0;
};

// ---------------------------------------------CHAINING HASH TABLE START---------------------------------------------
class ChainingHashTable : public HashTable
{
public:
    int hashNumber; // 1 = hash1, 2 = hash2
    vector<RedBlackTree *> table;

    ChainingHashTable(int size, int hashFunc);
    ~ChainingHashTable();

    int insert(const string &key, int value) override;
    void remove(const string &key) override;
    int search(const string &key) override;
    void clear() override;
};

ChainingHashTable::ChainingHashTable(int size, int hashFunc)
{
    maxSize = size;
    currentSize = 0;
    hashNumber = hashFunc;
    table.resize(maxSize, nullptr);
}

ChainingHashTable::~ChainingHashTable()
{
    clear();
}

int ChainingHashTable::insert(const string &key, int value)
{
    int index = (hashNumber == 1) ? hash1(key, maxSize) : hash2(key, maxSize);
    if (table[index] == nullptr)
    {
        table[index] = new RedBlackTree();
        table[index]->insert(key, value);
        currentSize++;
        // debug
        // cout << "Inserted key: " << key << ", value: " << value << " at index: " << index << endl;
        return 0; // no collision
    }
    else
    {
        if (table[index]->search(key)) // already exists, jodio age check korsi
        {
            return 0;
        }
        else
        {
            table[index]->insert(key, value);
            currentSize++;
            // cout << "Inserted key: " << key << ", value: " << value << " at index: " << index << endl;

            return 1; // collision occurred
        }
    }
}

int ChainingHashTable::search(const string &key)
{
    int index = (hashNumber == 1) ? hash1(key, maxSize) : hash2(key, maxSize);
    if (table[index] == nullptr)
        return -1;
    return table[index]->search(key) ? 1 : -1;
}

// removal er pore rb tree empty hole remove that rb tree too
void ChainingHashTable::remove(const string &key)
{
    int index = (hashNumber == 1) ? hash1(key, maxSize) : hash2(key, maxSize);
    if (table[index] != nullptr)
    {
        // check existence
        if (table[index]->search(key)) // rb tree er search will return true of false
        {
            table[index]->deleteNode(key);
            currentSize--;

            // rb tree empty hole null
            if (table[index]->isEmpty())
            {
                delete table[index];
                table[index] = nullptr;
            }
        }
    }
    // there is not even an rb tree, so element o nai
}

void ChainingHashTable::clear()
{
    for (int i = 0; i < maxSize; ++i)
    {
        if (table[i] != nullptr)
        {
            delete table[i];
            table[i] = nullptr;
        }
    }
    currentSize = 0;
}

// ---------------------------------------------CHAINING HASH TABLE END-----------------------------------------------
//
//

// ---------------------------------------------LINEAR PROBING HASH TABLE START---------------------------------------------

class LinearProbingHashTable : public HashTable
{
public:
    vector<pair<string, int> *> table;
    int hashNumber;                   // 1 = hash1, 2 = hash2
    int stepSize;                     // S , 3 or 5
    pair<string, int> *deletedMarker; // special entry

    LinearProbingHashTable(int size, int hashFunc, int step = 3)
    {
        maxSize = size;
        currentSize = 0;
        hashNumber = hashFunc;
        stepSize = step;
        table.resize(maxSize, nullptr);
        deletedMarker = new pair<string, int>("", -1);
    }

    ~LinearProbingHashTable()
    {
        clear();
        delete deletedMarker;
    }

    // insert wil lretturn total number of collsions not just one
    int insert(const string &key, int value) override
    {
        int hashVal = (hashNumber == 1) ? hash1(key, maxSize) : hash2(key, maxSize);
        int collisionCount = 0;

        for (int i = 0; i < maxSize; ++i)
        {
            // looping until maxsize because there can be atmost maxSize collisions
            int idx = (hashVal + i * stepSize) % maxSize;

            if (table[idx] == nullptr || table[idx] == deletedMarker)
            {
                table[idx] = new pair<string, int>(key, value);
                currentSize++;
                return collisionCount;
            }

            if (table[idx]->first == key)
            {
                // that key won't be inserted at all, so no collision
                return 0;
            }

            // not inserted, key not found, so must be a collision
            collisionCount++;
        }
        return collisionCount;
    }

    // (wasFound?, number of probes)).. keeping it like that because i might need to check whether the key was found or not in the online
    pair<bool, int> searchWithProbes(const string &key)
    {
        int hashVal = (hashNumber == 1) ? hash1(key, maxSize) : hash2(key, maxSize);

        for (int i = 0; i < maxSize; ++i)
        {
            // i+1 = number of probes
            int idx = (hashVal + i * stepSize) % maxSize;

            if (table[idx] == nullptr)
                return {false, i + 1}; // not found, probed i+1 times

            if (table[idx] != deletedMarker && table[idx]->first == key)
                return {true, i + 1}; // found in i+1 probes
        }

        return {false, maxSize}; // not found after full probing
    }
    int search(const string &key) override
    {
        // pure virutal function, so must implement kora lagbe, agertar nam change kora thk hoy nai
        pair<bool, int> result = searchWithProbes(key);
        return result.first ? result.second : -1;
    }

    void remove(const string &key) override
    {
        int hashVal = (hashNumber == 1) ? hash1(key, maxSize) : hash2(key, maxSize);

        for (int i = 0; i < maxSize; ++i)
        {
            int idx = (hashVal + i * stepSize) % maxSize;

            if (table[idx] == nullptr)
            {

                return;
            }

            if (table[idx] != deletedMarker && table[idx]->first == key)
            {
                // special marker
                delete table[idx];
                table[idx] = deletedMarker;
                currentSize--;
                return;
            }
        }
    }

    void clear() override
    {
        for (int i = 0; i < maxSize; ++i)
        {
            if (table[i] != nullptr && table[i] != deletedMarker)
                delete table[i];
            table[i] = nullptr;
        }
        currentSize = 0;
    }
};

// ---------------------------------------------LINEAR PROBING HASH TABLE END-----------------------------------------------
//
//
// ---------------------------------------------DOUBLE HASHING HASH TABLE START---------------------------------------------

class DoubleHashingHashTable : public HashTable
{
public:
    vector<pair<string, int> *> table;
    int hashNumber;
    // 1 - primary hash 1 , seconday hash2, and for 2 it is vice versa
    pair<string, int> *deletedMarker;

    DoubleHashingHashTable(int size, int hashFunc)
    {
        maxSize = size;
        currentSize = 0;
        hashNumber = hashFunc;
        table.resize(maxSize, nullptr);
        deletedMarker = new pair<string, int>("", -1);
    }

    ~DoubleHashingHashTable()
    {
        clear();
        delete deletedMarker;
    }

    int getHash1(const string &key)
    {
        return (hashNumber == 1) ? hash1(key, maxSize) : hash2(key, maxSize);
    }

    int getHash2(const string &key)
    {
        return (hashNumber == 1) ? hash2(key, maxSize) : hash1(key, maxSize);
    }
    // jukuno ekta ke primary hash hishebe use korbo as report needs two sets of data. not sure my interpretation is correct or not

    int insert(const string &key, int value) override
    {
        int h1 = getHash1(key);
        int h2 = getHash2(key);
        int collisionCount = 0;

        for (int i = 0; i < maxSize; ++i)
        {
            int idx = (h1 + i * h2) % maxSize;

            if (table[idx] == nullptr || table[idx] == deletedMarker)
            {
                table[idx] = new pair<string, int>(key, value);
                currentSize++;
                return collisionCount;
            }

            if (table[idx]->first == key)
            {
                return 0;
            }

            collisionCount++;
        }

        return collisionCount;
    }

    pair<bool, int> searchWithProbes(const string &key)
    {
        int h1 = getHash1(key);
        int h2 = getHash2(key);

        for (int i = 0; i < maxSize; ++i)
        {
            int idx = (h1 + i * h2) % maxSize;

            if (table[idx] == nullptr)
                return {false, i + 1};

            if (table[idx] != deletedMarker && table[idx]->first == key)
                return {true, i + 1};
        }

        return {false, maxSize};
    }

    int search(const string &key) override
    {
        pair<bool, int> result = searchWithProbes(key);
        return result.first ? result.second : -1;
    }

    void remove(const string &key) override
    {
        int h1 = getHash1(key);
        int h2 = getHash2(key);

        for (int i = 0; i < maxSize; ++i)
        {
            int idx = (h1 + i * max(1, h2)) % maxSize; // ensuring h2 ain't zero, nahole infinte loop...

            if (table[idx] == nullptr)
                return;

            if (table[idx] != deletedMarker && table[idx]->first == key)
            {
                delete table[idx];
                table[idx] = deletedMarker;
                currentSize--;
                return;
            }
        }
    }

    void clear() override
    {
        for (int i = 0; i < maxSize; ++i)
        {
            if (table[i] != nullptr && table[i] != deletedMarker)
                delete table[i];
            table[i] = nullptr;
        }
        currentSize = 0;
    }
};
// ---------------------------------------------DOUBLE HASHING HASH TABLE END---------------------------------------------

#endif