#include <bits/stdc++.h>
using namespace std;

struct Node
{
    int key;
    Node *left;
    Node *right;
    int height;
    // we could keep a parent pointer, but it's not a must for basic avl opearations as we will call the rotation functions from insert and delete functions directly who will have the parent node
    Node(int val)
    {
        key = val;
        left = right = nullptr;
        height = 1;
    }
};

class AVLTree
{
public:
    Node *root;

    int getHeight(Node *node)
    {
        if (node == nullptr)
            return 0;
        return node->height;
    }
    int getBalance(Node *node)
    {
        if (node == nullptr)
            return 0;
        return getHeight(node->left) - getHeight(node->right);
    }
    Node *rightRotate(Node *x)
    {
        if (!x->left)
        {
            return x;
        }
        Node *y = x->left;
        Node *T2 = y->right; // gotta keep track of this cause thie will be lost

        y->right = x;
        x->left = T2;

        x->height = 1 + max(getHeight(x->left), getHeight(x->right));
        y->height = 1 + max(getHeight(y->left), getHeight(y->right));
        return y;
    }
    Node *leftRotate(Node *x)
    {
        if (!x->right)
            return x;

        Node *y = x->right;
        Node *T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = 1 + max(getHeight(x->left), getHeight(x->right));
        y->height = 1 + max(getHeight(y->left), getHeight(y->right));

        return y;
    }
    Node *insert(Node *node, int key)
    {
        // Step 1: Perform normal BST insertion
        if (node == nullptr)
            return new Node(key);

        if (key < node->key)
        {
            node->left = insert(node->left, key);
        }
        else if (key > node->key)
        {
            node->right = insert(node->right, key);
        }
        else
        {
            // Equal keys not allowed in BST
            return node;
        }

        // Step 2: Update height of current node
        node->height = max(getHeight(node->left), getHeight(node->right)) + 1;

        // Step 3: Get balance factor
        int balanceFactor = getBalance(node);

        // Step 4: Handle imbalances
        // Left-Left case
        if (balanceFactor > 1 && key < node->left->key)
        {
            return rightRotate(node);
        }

        // Left-Right case
        if (balanceFactor > 1 && key > node->left->key)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right-Right case
        if (balanceFactor < -1 && key > node->right->key)
        {
            return leftRotate(node);
        }

        // Right-Left case
        if (balanceFactor < -1 && key < node->right->key)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        // No imbalance, return unchanged node
        return node;
    }
    Node *deleteNode(Node *node, int key)
    {
        if (!node)
            return node;
        if (node->key > key)
        {
            node->left = deleteNode(node->left, key);
        }
        else if (key > node->key)
        {
            node->right = deleteNode(node->right, key);
        }
        else
        {
            // we have got the node to delete
            if (!node->left && !node->right)
            {
                // we have to delete a node that has no child e.g. a leaf node
                Node *temp = node;
                node = nullptr;
                free(temp);
            }
            else if (!node->left)
            {
                // only right node ache
                Node *temp = node;
                node = node->right;
                free(temp);
            }
            else if (!node->right)
            {
                // only left child ache
                Node *temp = node;
                node = node->left;
                free(temp);
            }
            else
            {
                // duita child e ache. so indorder successor er node er value ta copy kore node e boshabo. ar oi inorder bandake delete korbo
                Node *insucc = minValueNode(node->right);
                node->key = insucc->key;
                node->right = deleteNode(node->right, node->key);
            }
        }
        // now we have delete the node. now we will see if we need to balance the tree
        if (!node)
            return node;
        // Step 2: Update height of current node
        node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
        // Step 3: Get balance factor
        int balanceFactor = getBalance(node);
        // Step 4: Handle imbalances
        // Left-Left case
        if (balanceFactor > 1 && getBalance(node->left) >= 0)
        {
            return rightRotate(node);
        }
        // Left-Right case
        if (balanceFactor > 1 && getBalance(node->left) < 0)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        // Right-Right case
        if (balanceFactor < -1 && getBalance(node->right) <= 0)
        {
            return leftRotate(node);
        }
        // Right-Left case
        if (balanceFactor < -1 && getBalance(node->right) > 0)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        // No imbalance, return unchanged node
        return node;
    }
    Node *minValueNode(Node *node)
    {
        Node *curr = node;
        while (curr->left)
        {
            curr = curr->left;
        }
        return curr;
    }
    void inOrder(Node *node)
    {
        if (node == nullptr)
            return;
        inOrder(node->left);
        cout << node->key << " ";
        inOrder(node->right);
    }

    AVLTree()
    {
        root = nullptr;
    }

    void insert(int key)
    {
        root = insert(root, key);
    }

    void remove(int key)
    {
        root = deleteNode(root, key);
    }

    void displayInOrder()
    {
        inOrder(root);
        cout << endl;
    }
};
