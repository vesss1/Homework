#include <iostream>
#include <string>
using namespace std;

class Node {
public:
    int data;
    Node* left;
    Node* right;

    Node(int x) {
        data = x;
        left = NULL;
        right = NULL;
    }
};

class BST {
private:
    Node* root;

    Node* insert(Node* node, int x) {
        if (node == NULL) {
            return new Node(x);
        }

        if (x < node->data) {
            node->left = insert(node->left, x);
        }
        else if (x > node->data) {
            node->right = insert(node->right, x);
        }

        return node;
    }

    Node* findMin(Node* node) {
        while (node->left != NULL) {
            node = node->left;
        }
        return node;
    }

    Node* remove(Node* node, int k) {
        if (node == NULL) {
            return NULL;
        }

        if (k < node->data) {
            node->left = remove(node->left, k);
        }
        else if (k > node->data) {
            node->right = remove(node->right, k);
        }
        else {
            if (node->left == NULL && node->right == NULL) {
                delete node;
                return NULL;
            }
            else if (node->left == NULL) {
                Node* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == NULL) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            else {
                Node* temp = findMin(node->right);
                node->data = temp->data;
                node->right = remove(node->right, temp->data);
            }
        }

        return node;
    }

    void inorder(Node* node) {
        if (node == NULL) {
            return;
        }

        inorder(node->left);
        cout << node->data << " ";
        inorder(node->right);
    }

    void clear(Node* node) {
        if (node == NULL) {
            return;
        }

        clear(node->left);
        clear(node->right);
        delete node;
    }

public:
    BST() {
        root = NULL;
    }

    ~BST() {
        clear(root);
    }

    void insert(int x) {
        root = insert(root, x);
    }

    void remove(int k) {
        root = remove(root, k);
    }

    void print() {
        inorder(root);
        cout << endl;
    }
};

int main() {
    BST tree;

    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);

    cout << "Before delete: ";
    tree.print();

    tree.remove(50);

    cout << "After delete: ";
    tree.print();

    return 0;
}
