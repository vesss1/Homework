#include <cmath>
#include <iostream>
#include <random>
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
        else {
            node->right = insert(node->right, x);
        }

        return node;
    }

    int height(Node* node) {
        if (node == NULL) {
            return 0;
        }

        int lh = height(node->left);
        int rh = height(node->right);

        if (lh > rh) {
            return lh + 1;
        }
        else {
            return rh + 1;
        }
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

    int getHeight() {
        return height(root);
    }
};

int main() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 1000000);

    cout << "n\theight\theight/log2(n)" << endl;

    for (int n = 100; n <= 10000; ) {
        BST tree;

        for (int i = 0; i < n; i++) {
            int x = dist(gen);
            tree.insert(x);
        }

        int h = tree.getHeight();
        double ratio = h / log2((double)n);

        cout << n << '\t' << h << '\t' << ratio << endl;

        if (n == 100) {
            n = 500;
        }
        else {
            n += 500;
        }
    }

    return 0;
}
