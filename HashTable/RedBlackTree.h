#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include <iostream>
#include <string>
using namespace std;

const int RED = 0;
const int BLACK = 1;

class Node
{
public:
    string key;
    int value;
    int color;
    Node *left, *right, *parent;

    Node(string key = "", int value = 0)
    {
        this->key = key;
        this->value = value;
        this->color = RED;
        left = right = parent = nullptr;
    }
};

class RedBlackTree
{
private:
    Node *root;
    Node *sentinel;

    void leftRotate(Node *x)
    {
        Node *y = x->right;
        x->right = y->left;
        if (y->left != sentinel)
            y->left->parent = x;
        y->parent = x->parent;
        if (x->parent == sentinel)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    void rightRotate(Node *x)
    {
        Node *y = x->left;
        x->left = y->right;
        if (y->right != sentinel)
            y->right->parent = x;
        y->parent = x->parent;
        if (x->parent == sentinel)
            root = y;
        else if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;
        y->right = x;
        x->parent = y;
    }

    void fixInsert(Node *z)
    {
        while (z->parent->color == RED)
        {
            if (z->parent == z->parent->parent->left)
            {
                Node *y = z->parent->parent->right;
                if (y->color == RED)
                {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else
                {
                    if (z == z->parent->right)
                    {
                        z = z->parent;
                        leftRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            }
            else
            {
                Node *y = z->parent->parent->left;
                if (y->color == RED)
                {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else
                {
                    if (z == z->parent->left)
                    {
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    void transplant(Node *u, Node *v)
    {
        if (u->parent == sentinel)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;
        v->parent = u->parent;
    }

    Node *minimum(Node *node)
    {
        while (node->left != sentinel)
            node = node->left;
        return node;
    }

    void fixDelete(Node *x)
    {
        while (x != root && x->color == BLACK)
        {
            if (x == x->parent->left)
            {
                Node *w = x->parent->right;
                if (w->color == RED)
                {
                    w->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == BLACK && w->right->color == BLACK)
                {
                    w->color = RED;
                    x = x->parent;
                }
                else
                {
                    if (w->right->color == BLACK)
                    {
                        w->left->color = BLACK;
                        w->color = RED;
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            }
            else
            {
                Node *w = x->parent->left;
                if (w->color == RED)
                {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == BLACK && w->left->color == BLACK)
                {
                    w->color = RED;
                    x = x->parent;
                }
                else
                {
                    if (w->left->color == BLACK)
                    {
                        w->right->color = BLACK;
                        w->color = RED;
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    void destroyTree(Node *node)
    {
        if (node == nullptr || node == sentinel)
            return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

    void inorderHelper(Node *node)
    {
        if (node != sentinel)
        {
            inorderHelper(node->left);
            cout << node->key << ":" << node->value << " (" << (node->color == RED ? "R" : "B") << ") ";
            inorderHelper(node->right);
        }
    }

public:
    RedBlackTree()
    {
        sentinel = new Node();
        sentinel->color = BLACK;
        sentinel->left = sentinel->right = sentinel->parent = sentinel;
        root = sentinel;
    }

    ~RedBlackTree()
    {
        destroyTree(root);
        delete sentinel;
    }

    void insert(string key, int value)
    {
        Node *z = new Node(key, value);
        z->left = z->right = z->parent = sentinel;

        Node *y = sentinel;
        Node *x = root;

        while (x != sentinel)
        {
            y = x;
            if (key < x->key)
                x = x->left;
            else if (key > x->key)
                x = x->right;
            else
            {
                x->value = value;
                delete z;
                return;
            }
        }

        z->parent = y;
        if (y == sentinel)
            root = z;
        else if (key < y->key)
            y->left = z;
        else
            y->right = z;

        fixInsert(z);
    }

    void deleteNode(string key)
    {
        Node *z = root;
        while (z != sentinel && z->key != key)
            z = (key < z->key) ? z->left : z->right;

        if (z == sentinel)
        {
            cout << "Key \"" << key << "\" not found.\n";
            return;
        }

        Node *y = z;
        int yOriginalColor = y->color;
        Node *x;

        if (z->left == sentinel)
        {
            x = z->right;
            transplant(z, z->right);
        }
        else if (z->right == sentinel)
        {
            x = z->left;
            transplant(z, z->left);
        }
        else
        {
            y = minimum(z->right);
            yOriginalColor = y->color;
            x = y->right;
            if (y->parent == z)
                x->parent = y;
            else
            {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        delete z;
        if (yOriginalColor == BLACK)
            fixDelete(x);
    }

    bool search(string key)
    {
        Node *current = root;
        while (current != sentinel)
        {
            if (key == current->key)
                return true;
            else if (key < current->key)
                current = current->left;
            else
                current = current->right;
        }
        return false;
    }

    int getValue(string key)
    {
        Node *current = root;
        while (current != sentinel)
        {
            if (key == current->key)
                return current->value;
            else if (key < current->key)
                current = current->left;
            else
                current = current->right;
        }
        cerr << "Error: Key \"" << key << "\" not found.\n";
        return -1;
    }

    void printTree()
    {
        cout << "Tree (inorder): ";
        inorderHelper(root);
        cout << endl;
    }

    bool isEmpty()
    {
        return root == sentinel;
    }

    void clear()
    {
        destroyTree(root);
        root = sentinel;
    }
};
#endif
