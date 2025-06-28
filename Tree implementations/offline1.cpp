#include "color.hpp"
#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <iomanip>
using namespace std;
ifstream infile("input.txt");
ofstream outfile("output.txt");
class Node
{
public:
    int id; // id is key in that case
    string prodName;
    int quantity;
    Node *left, *right, *parent;
    int color; // 0 for red, 1 for black
    Node(int id, string prodName, int quantity)
    {
        this->id = id;
        this->prodName = prodName;
        this->quantity = quantity;
        left = right = parent = nullptr;
    }
};
void print(string s, int color)
{
    if (color == 1)
    {
        // black
        cout << dye::black(s);
        outfile << s;
    }
    else
    {
        // red
        cout << dye::red(s);
        outfile << s;
    }
}

class RedBlackTree
{
public:
    Node *root;
    Node *sentinel;
    int size;
    RedBlackTree()
    {
        sentinel = new Node(-1, "sentinel", 0);
        sentinel->color = 1;
        sentinel->left = sentinel->right = sentinel->parent = sentinel;
        root = sentinel;
        size = 0;
    }
    int isExist(int id)
    {
        // exist korle quantity return korbo nahole 0
        Node *curr = root;
        while (curr != sentinel)
        {
            if (curr->id > id)
            {
                curr = curr->left;
            }
            else if (curr->id < id)
            {
                curr = curr->right;
            }
            else
            {
                // got him
                return curr->quantity;
            }
        }
        return 0;
    }
    void leftRotate(Node *x)
    {
        if (x == nullptr || x->right == sentinel)
            return; // no need
        Node *y = x->right;
        x->right = y->left;
        if (y->left != sentinel)
        {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == sentinel)
        {
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
    void rightRotate(Node *x)
    {
        if (x == nullptr || x->left == sentinel)
            return;
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

    // Node *treeMinimum(Node *x)
    // {
    //     while (x->left != sentinel)
    //         x = x->left;
    //     return x;
    // }
    Node *treeMaximum(Node *x)
    {
        while (x->right != sentinel)
            x = x->right;
        return x;
    }

    void deleteFix(Node *x)
    {
        while (x != root && x->color == 1)
        {
            if (x == x->parent->left)
            {
                Node *w = x->parent->right;
                if (w->color == 0)
                {
                    w->color = 1;
                    x->parent->color = 0;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == 1 && w->right->color == 1)
                {
                    w->color = 0;
                    x = x->parent;
                }
                else
                {
                    if (w->right->color == 1)
                    {
                        w->left->color = 1;
                        w->color = 0;
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = 1;
                    w->right->color = 1;
                    leftRotate(x->parent);
                    x = root;
                }
            }
            else
            {
                Node *w = x->parent->left;
                if (w->color == 0)
                {
                    w->color = 1;
                    x->parent->color = 0;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == 1 && w->left->color == 1)
                {
                    w->color = 0;
                    x = x->parent;
                }
                else
                {
                    if (w->left->color == 1)
                    {
                        w->right->color = 1;
                        w->color = 0;
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = 1;
                    w->left->color = 1;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = 1;
    }

    void deleteFromTree(int id)
    {
        Node *z = root;
        while (z != sentinel)
        {
            if (z->id == id)
                break;
            else if (id < z->id)
                z = z->left;
            else
                z = z->right;
        }

        if (z == sentinel)
            return; // ivalid key. does not exist

        Node *y = z;
        int prevYColor = y->color;
        Node *x;

        if (z->right == sentinel)
        {
            x = z->left;
            transplant(z, z->left);
        }
        else if (z->left == sentinel)
        {
            x = z->right;
            transplant(z, z->right);
        }
        else
        {
            y = treeMaximum(z->left);
            prevYColor = y->color;
            x = y->left;
            if (y->parent == z)
            {
                x->parent = y;
            }
            else
            {
                transplant(y, y->left);
                y->left = z->left;
                y->left->parent = y;
            }
            transplant(z, y);
            y->right = z->right;
            y->right->parent = y
            y->color = z->color;
        }

        delete z;
        size--;

        if (prevYColor == 1)
        {
            deleteFix(x);
        }
    }

    void fixInsert(Node *z)
    {
        // following the cormen books algo
        while (z->parent->color == 0)
        {
            if (z->parent == z->parent->parent->left) // left child
            {
                Node *y = z->parent->parent->right; // uncle
                if (y->color == 0)
                {
                    z->parent->color = 1;
                    y->color = 1;
                    z->parent->parent->color = 0;
                    z = z->parent->parent;
                }
                else
                {
                    if (z == z->parent->right)
                    {
                        z = z->parent;
                        leftRotate(z);
                    }
                    z->parent->color = 1;
                    z->parent->parent->color = 0;
                    rightRotate(z->parent->parent);
                }
            }
            else // symmetric case
            {
                Node *y = z->parent->parent->left;
                if (y->color == 0)
                {
                    z->parent->color = 1;
                    y->color = 1;
                    z->parent->parent->color = 0;
                    z = z->parent->parent;
                }
                else
                {
                    if (z == z->parent->left)
                    {
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = 1;
                    z->parent->parent->color = 0;
                    leftRotate(z->parent->parent);
                }
            }
            if (z == root)
                break;
        }
        root->color = 1;
    }
    void insertIntoTree(int id, string prodName, int quantity)
    {
        // normal insertion
        // first we gotta see if this id node is alreayd in the tree or not. if it is, then we will just update the name and quantity.
        Node *current = root;
        Node *parent = sentinel;

        while (current != sentinel)
        {
            parent = current;
            if (id < current->id)
            {
                current = current->left;
            }
            else if (id > current->id)
            {
                current = current->right;
            }
            else
            {
                if (prodName != "")
                {
                    current->prodName = prodName;
                }
                current->quantity = quantity;
                return;
            }
        }
        Node *newNode = new Node(id, prodName, quantity);

        newNode->parent = parent;
        if (parent == sentinel)
        {
            root = newNode; // emtpy tree
        }
        else if (id < parent->id)
        {
            parent->left = newNode;
        }
        else
        {
            parent->right = newNode;
        }
        newNode->left = sentinel;
        newNode->right = sentinel;
        newNode->color = 0; // gotta be red
        fixInsert(newNode);
        size++;
    }

    void inorderHelper(Node *node)
    {
        if (node == sentinel || node == nullptr)
        {
            // cout << "came to null" << endl;
            return;
        }

        inorderHelper(node->left);
        print(to_string(node->id) + " => " + node->prodName + " (" + to_string(node->quantity) + ")", node->color);
        cout << endl;
        outfile << endl;
        inorderHelper(node->right);
    }
    void inorder()
    {
        inorderHelper(root);
    }
    void dfs(Node *node)
    {
        if (node == sentinel)
            return;
        print(to_string(node->id) + "_" + node->prodName, node->color);
        if (node->left != sentinel || node->right != sentinel)
        {
            cout << "(";
            outfile << "(";
            if (node->left != sentinel)
                dfs(node->left);
            else
            {
                cout << "";
                outfile << "";
            }

            cout << ",";
            outfile << ",";
            if (node->right != sentinel)
                dfs(node->right);
            else
            {
                cout << "";
                outfile << "";
            }

            cout << ")";
            outfile << ")";
        }
    }

    void paranthesisPrint()
    {
        dfs(root);
        cout << endl;
        outfile << endl;
    }
    void deleteTree(Node *node)
    {
        if (node == sentinel)
            return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
    ~RedBlackTree()
    {
        deleteTree(root);
        delete sentinel;
    }
};
class InventorySystem
{

public:
    RedBlackTree tree;

    // constructor
    InventorySystem()
    {
        tree = RedBlackTree();
        // cout << "ddd" << endl;
    }

    void addItem(int id, string prodName, int quantity)
    {
        tree.insertIntoTree(id, prodName, quantity);
    }
    void BuyItem(int id, int quantity)
    {
        int currentQuantity = tree.isExist(id);
        if (currentQuantity == 0)
        {
            // cout << "Item not available" << endl;
            return;
        }

        if (currentQuantity <= quantity)
        {
            tree.deleteFromTree(id);
            return;
        }
        tree.insertIntoTree(id, "", currentQuantity - quantity);
    }
    bool checkItem(int id)
    {
        return tree.isExist(id) > 0;
    }
    int InventorySize()
    {
        return tree.size;
    }
    bool Empty()
    {
        return tree.size == 0;
    }
    void ClearInventory()
    {
        if (tree.size == 0)
        {
            cout << "unsuccessful" << endl;
            outfile << "unsuccessful" << endl;
            return;
        }
        tree.deleteTree(tree.root);
        tree.root = tree.sentinel;
        tree.size = 0;
        cout << "successful" << endl;
        outfile << "successful" << endl;
    }
    void ListInventory()
    {
        tree.inorder();
    }
};
int main()
{
    InventorySystem inventory;

    string line;
    while (getline(infile, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);
        string command;
        ss >> command;

        if (command == "Clr")
        {
            inventory.ClearInventory();
        }
        else if (command == "AI")
        {
            int id, quantity;
            string prodName;

            ss >> id;
            ss >> ws;
            if (ss.peek() == '"')
            {
                ss.get();
                getline(ss, prodName, '"');
            }
            else
            {
                ss >> prodName;
            }
            ss >> quantity;

            inventory.addItem(id, prodName, quantity);
            inventory.tree.paranthesisPrint();
        }
        else if (command == "BI")
        {
            int id, quantity;
            ss >> id >> quantity;
            inventory.BuyItem(id, quantity);
            inventory.tree.paranthesisPrint();
        }
        else if (command == "CI")
        {
            int id;
            ss >> id;
            if (inventory.checkItem(id))
            {
                cout << "Stock left: " << inventory.tree.isExist(id) << endl;
                outfile << "Stock left: " << inventory.tree.isExist(id) << endl;
            }
            else
            {
                cout << "Not available" << endl;
                outfile << "Not available" << endl;
            }
        }
        else if (command == "S")
        {
            cout << inventory.InventorySize() << endl;
            outfile << inventory.InventorySize() << endl;
        }
        else if (command == "Em")
        {
            cout << (inventory.Empty() ? "yes" : "no") << endl;
            outfile << (inventory.Empty() ? "yes" : "no") << endl;
        }
        else if (command == "Itr")
        {
            inventory.ListInventory();
        }
    }

    infile.close();
    outfile.close();
    return 0;
}
