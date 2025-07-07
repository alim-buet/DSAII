#include <iostream>
using namespace std;

const int RED = 0;
const int BLACK = 1;

class Node
{
public:
    int key;
    int color;
    Node *left, *right, *parent;

    Node(int key = 0)
    {
        this->key = key;
        this->color = RED;
        left = right = parent = nullptr;
    }
};

class RedBlackTree
{
private:
    Node *root;
    Node *sentinel;

    // Utility to perform left rotation
    void leftRotate(Node *x)
    {
        if (x == sentinel || x->right == sentinel)
            return; // Cannot rotate

        Node *y = x->right;
        x->right = y->left;

        if (y->left != sentinel)
        {
            y->left->parent = x;
        }

        y->parent = x->parent;

        if (x->parent == sentinel)
        {
            // x was root, so y becomes the new root
            root = y;
        }
        else if (x == x->parent->left)
        {
            x->parent->left = y;
        }
        else
        {
            x->parent->right = y;
        }

        y->left = x;
        x->parent = y;
    }

    // Utility to perform right rotation
    void rightRotate(Node *x)
    {
        if (x == sentinel || x->left == sentinel)
            return; // Cannot rotate

        Node *y = x->left;
        x->left = y->right;

        if (y->right != sentinel)
        {
            y->right->parent = x;
        }

        y->parent = x->parent;

        if (x->parent == sentinel)
        {
            root = y;
        }
        else if (x == x->parent->right)
        {
            x->parent->right = y;
        }
        else
        {
            x->parent->left = y;
        }

        y->right = x;
        x->parent = y;
    }

    // Fixing tree after insertion
    void fixInsert(Node *z)
    {
        while (z != root && z->parent != sentinel && z->parent->color == RED)
        {
            if (z->parent->parent == sentinel)
                break; // Safety check

            if (z->parent == z->parent->parent->left)
            {
                Node *y = z->parent->parent->right; // Uncle
                if (y != sentinel && y->color == RED)
                {
                    // Case 1: Uncle is red
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else
                {
                    if (z == z->parent->right)
                    {
                        // Case 2: z is right child
                        z = z->parent;
                        leftRotate(z);
                    }
                    // Case 3: z is left child
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            }
            else
            {
                // Mirror case: parent is right child
                Node *y = z->parent->parent->left; // Uncle
                if (y != sentinel && y->color == RED)
                {
                    // Case 1 mirror: Uncle is red
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                }
                else
                {
                    if (z == z->parent->left)
                    {
                        // Case 2 mirror: z is left child
                        z = z->parent;
                        rightRotate(z);
                    }
                    // Case 3 mirror: z is right child
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    // Fixing tree after deletion
    void fixDelete(Node *x)
    {
        while (x != root && x->color == BLACK)
        {
            if (x == x->parent->left)
            {
                Node *w = x->parent->right;

                // Case 1: Sibling is red
                if (w->color == RED)
                {
                    w->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }

                // Case 2: Sibling is black with two black children
                if ((w->left == sentinel || w->left->color == BLACK) &&
                    (w->right == sentinel || w->right->color == BLACK))
                {
                    w->color = RED;
                    x = x->parent;
                }
                else
                {
                    // Case 3: Sibling is black with red left child and black right child
                    if (w->right == sentinel || w->right->color == BLACK)
                    {
                        if (w->left != sentinel)
                            w->left->color = BLACK;
                        w->color = RED;
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    // Case 4: Sibling is black with red right child
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (w->right != sentinel)
                        w->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            }
            else
            {
                // Mirror case: x is right child
                Node *w = x->parent->left;

                // Case 1 mirror: Sibling is red
                if (w->color == RED)
                {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }

                // Case 2 mirror: Sibling is black with two black children
                if ((w->left == sentinel || w->left->color == BLACK) &&
                    (w->right == sentinel || w->right->color == BLACK))
                {
                    w->color = RED;
                    x = x->parent;
                }
                else
                {
                    // Case 3 mirror: Sibling is black with red right child and black left child
                    if (w->left == sentinel || w->left->color == BLACK)
                    {
                        if (w->right != sentinel)
                            w->right->color = BLACK;
                        w->color = RED;
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    // Case 4 mirror: Sibling is black with red left child
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (w->left != sentinel)
                        w->left->color = BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    // Helper function to replace one subtree with another
    void transplant(Node *u, Node *v)
    {
        if (u->parent == sentinel)
        {
            root = v;
        }
        else if (u == u->parent->left)
        {
            u->parent->left = v;
        }
        else
        {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    // Find minimum node in subtree
    Node *minimum(Node *node)
    {
        while (node->left != sentinel)
            node = node->left;
        return node;
    }

    // Delete all nodes (for destructor)
    void destroyTree(Node *node)
    {
        if (node == sentinel)
            return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

    // Inorder print
    void inorderHelper(Node *node)
    {
        if (node != sentinel)
        {
            inorderHelper(node->left);
            cout << node->key << " (" << (node->color == RED ? "R" : "B") << ") ";
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

    void insert(int key)
    {
        Node *z = new Node(key);
        z->left = z->right = sentinel;

        Node *y = sentinel;
        Node *x = root;

        // Find the appropriate position for insertion
        while (x != sentinel)
        {
            y = x;
            if (z->key < x->key)
                x = x->left;
            else
                x = x->right;
        }

        z->parent = y;
        if (y == sentinel)
        {
            root = z;
        }
        else if (z->key < y->key)
        {
            y->left = z;
        }
        else
        {
            y->right = z;
        }

        // Fix any violations of red-black tree properties
        fixInsert(z);
    }

    void deleteNode(int key)
    {
        Node *z = root;

        // Find the node to delete
        while (z != sentinel && z->key != key)
        {
            z = (key < z->key) ? z->left : z->right;
        }

        if (z == sentinel)
        {
            cout << "Key " << key << " not found in the tree.\n";
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
            {
                x->parent = y;
            }
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

        // Fix any violations of red-black tree properties
        if (yOriginalColor == BLACK)
            fixDelete(x);
    }

    void printTree()
    {
        cout << "Tree (inorder): ";
        inorderHelper(root);
        cout << endl;
    }

    // Search function
    bool search(int key)
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

    // Check if tree is empty
    bool isEmpty()
    {
        return root == sentinel;
    }
};


