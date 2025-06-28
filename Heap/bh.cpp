#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <vector>
#include <queue>
#include <climits>
#include <unordered_map>
using namespace std;

ofstream outputFile;

class Node
{
public:
    int key;
    Node *left_most_child;
    Node *right_sibling;
    Node *parent;
    Node(int k)
    {
        key = k;
        left_most_child = nullptr;
        right_sibling = nullptr;
        parent = nullptr;
    }
};

class BinomialTree
{
public:
    Node *root;
    int degree;
    BinomialTree(int k)
    {
        // sometimes we need to create a tree with a single node, oi time e just node value pass korlei tree create korbo
        root = new Node(k);
        degree = 0;
    }

    void combineTree(BinomialTree *other)
    {
        if (degree != other->degree)
        {
            return;
        }
        if (root->key < other->root->key)
        {
            other->root->right_sibling = root->left_most_child;
            if (other->root->right_sibling)
                other->root->right_sibling->parent = other->root;
            root->left_most_child = other->root;
            other->root->parent = root;
        }
        else
        {
            root->right_sibling = other->root->left_most_child;
            if (root->right_sibling)
                root->right_sibling->parent = root;
            other->root->left_most_child = root;
            root->parent = other->root;
            root = other->root;
        }
        degree++;
    }
};

class BinomialHeap
{
public:
    vector<BinomialTree *> trees;
    unordered_map<int, vector<Node *>> keyToNodes; // considering multiple nodes with same key

    BinomialHeap() {}

    // sometimes we gotta create a heap with single node
    BinomialHeap(int k)
    {
        BinomialTree *tree = new BinomialTree(k);
        trees.push_back(tree);
        keyToNodes[k].push_back(tree->root);
    }

    void siftUp(Node *node)
    {
        while (node->parent != nullptr && node->key < node->parent->key)
        {
            int oldKey = node->key;
            int parentOldKey = node->parent->key;

            int tempKey = node->key;
            node->key = node->parent->key;
            node->parent->key = tempKey;

            removeNodeFromKey(oldKey, node);
            removeNodeFromKey(parentOldKey, node->parent);
            keyToNodes[node->key].push_back(node);
            keyToNodes[node->parent->key].push_back(node->parent);

            node = node->parent;
        }
    }

    void removeNodeFromKey(int key, Node *node)
    {
        if (keyToNodes.find(key) != keyToNodes.end())
        {
            auto &nodes = keyToNodes[key];
            for (int i = 0; i < nodes.size(); i++)
            {
                if (nodes[i] == node)
                {
                    nodes.erase(nodes.begin() + i);
                    break;
                }
            }
            if (nodes.empty())
            {
                keyToNodes.erase(key);
            }
        }
    }

    void updateKey(int oldKey, int newKey)
    {
        if (keyToNodes.find(oldKey) == keyToNodes.end() || keyToNodes[oldKey].empty())
        {
            return;
        }

        vector<Node *> nodesToUpdate = keyToNodes[oldKey];
        keyToNodes.erase(oldKey);

        for (Node *node : nodesToUpdate)
        {
            node->key = newKey;
            keyToNodes[newKey].push_back(node);
            siftUp(node);
        }
    }

    void insert(int k)
    {
        BinomialHeap *newHeap = new BinomialHeap(k);
        combineHeap(newHeap);
    }

    int findMin()
    {
        if (trees.empty())
        {
            return -1;
        }
        int minElement = INT_MAX;
        for (int i = 0; i < trees.size(); i++)
        {
            if (trees[i] != nullptr && trees[i]->root->key < minElement)
            {
                minElement = trees[i]->root->key;
            }
        }
        return (minElement == INT_MAX) ? -1 : minElement;
    }

    void combineHeap(BinomialHeap *other)
    {
        for (auto &pair : other->keyToNodes)
        {
            for (Node *node : pair.second)
            {
                keyToNodes[pair.first].push_back(node);
            }
        }

        vector<BinomialTree *> newTrees;
        BinomialTree *carry = nullptr;

        int maxSize = max(trees.size(), other->trees.size());
        for (int i = 0; i < maxSize || carry != nullptr; i++)
        {
            BinomialTree *t1 = (i < trees.size()) ? trees[i] : nullptr;
            BinomialTree *t2 = (i < other->trees.size()) ? other->trees[i] : nullptr;

            int count = (t1 != nullptr) + (t2 != nullptr) + (carry != nullptr);

            if (count == 0)
            {
                newTrees.push_back(nullptr);
            }
            else if (count == 1)
            {
                newTrees.push_back(t1 ? t1 : (t2 ? t2 : carry));
                carry = nullptr;
            }
            else if (count == 2)
            {
                if (!t1)
                {
                    t2->combineTree(carry);
                    carry = t2;
                    newTrees.push_back(nullptr);
                }
                else if (!t2)
                {
                    t1->combineTree(carry);
                    carry = t1;
                    newTrees.push_back(nullptr);
                }
                else
                {
                    t1->combineTree(t2);
                    carry = t1;
                    newTrees.push_back(nullptr);
                }
            }
            else
            {
                t1->combineTree(t2);
                newTrees.push_back(carry);
                carry = t1;
            }
        }

        trees = newTrees;
    }

    int extractMin()
    {
        if (trees.empty())
        {
            return -1;
        }

        int minIndex = -1;
        int minElement = INT_MAX;
        for (int i = 0; i < trees.size(); i++)
        {
            if (trees[i] != nullptr && trees[i]->root->key < minElement)
            {
                minElement = trees[i]->root->key;
                minIndex = i;
            }
        }

        if (minIndex == -1)
        {
            return -1;
        }

        removeNodeFromKey(minElement, trees[minIndex]->root);

        BinomialTree *minTree = trees[minIndex];
        trees[minIndex] = nullptr;

        BinomialHeap *newHeap = new BinomialHeap();

        vector<BinomialTree *> childTrees;
        Node *child = minTree->root->left_most_child;
        int degree = minTree->degree - 1;

        while (child != nullptr)
        {
            Node *next = child->right_sibling;

            BinomialTree *tree = new BinomialTree(child->key);
            delete tree->root;
            tree->root = child;
            tree->degree = degree;
            degree--;
            child->right_sibling = nullptr;
            child->parent = nullptr;

            newHeap->keyToNodes[child->key].push_back(child);

            childTrees.push_back(tree);
            child = next;
        }

        reverse(childTrees.begin(), childTrees.end());
        newHeap->trees = childTrees;

        combineHeap(newHeap);

        delete minTree;
        return minElement;
    }

    void removeKey(int k)
    {
        if (keyToNodes.find(k) == keyToNodes.end() || keyToNodes[k].empty())
        {
            return;
        }

        while (keyToNodes.find(k) != keyToNodes.end() && !keyToNodes[k].empty())
        {
            Node *node = keyToNodes[k][0];
            removeNodeFromKey(k, node);
            node->key = INT_MIN;
            keyToNodes[INT_MIN].push_back(node);
            siftUp(node);
            extractMin();
        }
    }

    void Print()
    {
        cout << "Printing Binomial Heap..." << endl;
        outputFile << "Printing Binomial Heap..." << endl;

        for (int i = 0; i < trees.size(); i++)
        {
            if (trees[i] == nullptr)
                continue;

            cout << "Binomial Tree, B" << i << endl;
            outputFile << "Binomial Tree, B" << i << endl;

            queue<pair<Node *, int>> q;
            q.push({trees[i]->root, 0});

            int currentLevel = -1;
            vector<int> levelNodes;

            while (!q.empty())
            {
                Node *node = q.front().first;
                int level = q.front().second;
                q.pop();

                if (level != currentLevel)
                {
                    if (currentLevel != -1)
                    {
                        cout << "Level " << currentLevel << ": ";
                        outputFile << "Level " << currentLevel << ": ";
                        for (int j = 0; j < levelNodes.size(); j++)
                        {
                            cout << levelNodes[j];
                            outputFile << levelNodes[j];
                            if (j < levelNodes.size() - 1)
                            {
                                cout << " ";
                                outputFile << " ";
                            }
                        }
                        cout << endl;
                        outputFile << endl;
                    }

                    currentLevel = level;
                    levelNodes.clear();
                }

                levelNodes.push_back(node->key);

                Node *child = node->left_most_child;
                while (child != nullptr)
                {
                    q.push({child, level + 1});
                    child = child->right_sibling;
                }
            }

            if (currentLevel != -1)
            {
                cout << "Level " << currentLevel << ": ";
                outputFile << "Level " << currentLevel << ": ";
                for (int j = 0; j < levelNodes.size(); j++)
                {
                    cout << levelNodes[j];
                    outputFile << levelNodes[j];
                    if (j < levelNodes.size() - 1)
                    {
                        cout << " ";
                        outputFile << " ";
                    }
                }
                cout << endl;
                outputFile << endl;
            }
        }
    }
};

int main()
{
    BinomialHeap heap;
    ifstream inputFile("input.txt");
    outputFile.open("output.txt");

    string line;
    while (getline(inputFile, line))
    {
        istringstream iss(line);
        string command;
        iss >> command;

        if (command == "I")
        {
            int k;
            iss >> k;
            heap.insert(k);
        }
        else if (command == "P")
        {
            heap.Print();
        }
        else if (command == "R")
        {
            int k;
            iss >> k;
            heap.removeKey(k);
        }
        else if (command == "D")
        {
            int oldKey, newKey;
            iss >> oldKey >> newKey;
            heap.updateKey(oldKey, newKey);
        }
        else if (command == "F")
        {
            int result = heap.findMin();
            cout << "Find Min returned: " << result << endl;
            outputFile << "Find Min returned: " << result << endl;
        }
        else if (command == "E")
        {
            int result = heap.extractMin();
            cout << "Extract Min returned: " << result << endl;
            outputFile << "Extract Min returned: " << result << endl;
        }
    }

    inputFile.close();
    outputFile.close();
    return 0;
}
