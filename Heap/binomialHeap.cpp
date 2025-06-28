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
    unordered_map<int, Node *> keyToNode; // Map to store key -> node mapping for O(1) access

    BinomialHeap()
    {
        // size = 0;
    }

    // sometimes we gotta create a heap with single node
    BinomialHeap(int k)
    {
        BinomialTree *tree = new BinomialTree(k);
        trees.push_back(tree);
        keyToNode[k] = tree->root;
    }

    void siftUp(Node *node)
    {
        while (node->parent != nullptr && node->key < node->parent->key)
        {
            // key swap
            int tempKey = node->key;
            node->key = node->parent->key;
            node->parent->key = tempKey;

            // map update
            keyToNode[node->key] = node;
            keyToNode[node->parent->key] = node->parent;

            node = node->parent;
        }
    }
    // considering only unique keys exists in the heap

    void updateKey(int oldKey, int newKey)
    {
        // availablity chekcing
        if (keyToNode.find(oldKey) == keyToNode.end())
        {
            cout << "Key " << oldKey << " not found in heap" << endl;
            return;
        }

        // check if new key already exists
        if (keyToNode.find(newKey) != keyToNode.end())
        {
            cout << "New key " << newKey << " already exists in heap" << endl;
            return;
        }

        Node *node = keyToNode[oldKey]; // the node to update

        keyToNode.erase(oldKey);
        node->key = newKey;
        keyToNode[newKey] = node;

        siftUp(node);
    }

    void insert(int k)
    {
        if (keyToNode.find(k) != keyToNode.end())
        {
            cout << "Key " << k << " already exists in heap" << endl;
            return;
        }

        BinomialHeap *newHeap = new BinomialHeap(k);
        // update the map
        keyToNode[k] = newHeap->trees[0]->root;
        combineHeap(newHeap);
    }

    int findMin()
    {
        if (trees.empty())
        {
            cout << "empty heap" << endl;
            return -1;
        }
        int minElement = INT_MAX;
        for (int i = 0; i < trees.size(); i++)
        {
            // trees[i] = valid tree->if Bi exists, otherwise it will be null
            if (trees[i] != nullptr && trees[i]->root->key < minElement)
            {
                minElement = trees[i]->root->key;
            }
        }
        return (minElement == INT_MAX) ? -1 : minElement;
    }

    // Helper function to update the map when combining heaps
    // void updateMapFromHeap(BinomialHeap *other)
    // {
    //     for (auto &pair : other->keyToNode)
    //     {
    //         keyToNode[pair.first] = pair.second;
    //     }
    // }

    void combineHeap(BinomialHeap *other)
    {
        // // First, update our map with nodes from other heap
        // updateMapFromHeap(other);

        vector<BinomialTree *> newTrees;
        BinomialTree *carry = nullptr;

        int maxSize = max(trees.size(), other->trees.size());
        for (int i = 0; i < maxSize || carry != nullptr; i++)
        {
            // trees[i] = nullptr mane Bi main heap e nei, same goes for other->trees[i]
            // and newTrees will contain the trees of the combined heap
            // newTrees[i] - the tree of degree i in the combined heap. if does not exist then it will be nullptr
            BinomialTree *t1 = (i < trees.size()) ? trees[i] : nullptr;
            BinomialTree *t2 = (i < other->trees.size()) ? other->trees[i] : nullptr;

            int count = (t1 != nullptr) + (t2 != nullptr) + (carry != nullptr);

            if (count == 0)
            {
                newTrees.push_back(nullptr);
            }
            else if (count == 1)
            {
                // only one Bi, no collision, just push it to newTrees
                newTrees.push_back(t1 ? t1 : (t2 ? t2 : carry));
                carry = nullptr;
            }
            else if (count == 2)
            {
                // if we have two trees of i degree in a step, then in the final heap there will be no tree of  degree i rather i+1
                if (!t1)
                {
                    // t2 and carry of degree i is here
                    t2->combineTree(carry);
                    carry = t2;
                    newTrees.push_back(nullptr);
                }
                else if (!t2)
                {
                    // t1 and carry of degree i is here
                    t1->combineTree(carry);
                    carry = t1;
                    newTrees.push_back(nullptr);
                }
                else // both t1 and t2 are not null
                {
                    t1->combineTree(t2);
                    carry = t1;
                    newTrees.push_back(nullptr);
                }
            }
            else // count == 3
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
        // Check if heap is empty
        if (trees.empty())
        {
            cout << "Heap is empty" << endl;
            return -1;
        }
        // finding min key

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

        // we are gonna delete that min tree, so we need to remove the key from the map
        keyToNode.erase(minElement);

        // take the min tree
        BinomialTree *minTree = trees[minIndex];
        trees[minIndex] = nullptr; // now that tree is not part of the heap
        int k = minTree->degree;   // degree of the min tree

        BinomialHeap *newHeap = new BinomialHeap(); // this heap will be formed by combining the child tree of the deleted node

        vector<BinomialTree *> childTrees;            // will store the child trees. childtrees[i] will store the tree of degree i
        Node *child = minTree->root->left_most_child; // we are starting from the leftmost child (tree) so this gotta be the larges if the lot
        // lets say the tree from which we are extcting the min has degree of k, then removing the root of the tree will result in k child tree of B0,B1,...Bk-1. we will start from the leftmost Bk-1
        while (child != nullptr)
        {
            Node *next = child->right_sibling;

            // Each child becomes root of a new tree
            BinomialTree *tree = new BinomialTree(0);
            tree->root = child;
            tree->degree = --k; // from left to right k-1,k-2,....0
            child->right_sibling = nullptr;
            child->parent = nullptr;

            // just for mainting consistency
            newHeap->keyToNode[child->key] = child;

            childTrees.push_back(tree);
            child = next;
        }

        reverse(childTrees.begin(), childTrees.end()); // now Bk-1 will be at index 0, Bk-2 at index 1, and so on but we need B0,B1, Bn-1
        newHeap->trees = childTrees;

        combineHeap(newHeap);

        delete minTree;
        return minElement;
    }
    void removeKey(int k)
    {
        // gotta check if the key exists
        if (keyToNode.find(k) == keyToNode.end())
        {
            cout << "Key " << k << " not found in heap" << endl;
            return;
        }
        // go to the node
        Node *node = keyToNode[k];
        // make it negative inf
        node->key = INT_MIN;
        siftUp(node);
        extractMin();
        keyToNode.erase(k);
    }
    void Print()
    {
        // level by lever printer for each tree
        // Binomial Tree, B0
        // Level 0 : 10
        // Binomial Tree,B1
        // Level 0 : 2
        // Level 1 : 5
        for (int i = 0; i < trees.size(); i++)
        {
            // i is the order of the binomial tree
            if (trees[i] == nullptr)
                continue; // Bi does not exist
            cout << "Binomial Tree, B" << i << endl;
            // vector<Node *> currentLevel;
            // currentLevel.push_back(trees[i]->root);
            Node *currentLeftMostChild = trees[i]->root;
            int level = 0;
            while (currentLeftMostChild->left_most_child)
            {
                cout << "Level " << level << " : ";
                Node *it = currentLeftMostChild;
                while (it)
                {
                    cout << it->key << " ";
                    it = it->right_sibling;
                }
                cout << endl;
                level++;
                currentLeftMostChild = currentLeftMostChild->left_most_child;
            }
        }
    }
};