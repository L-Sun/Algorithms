#pragma once
#include <memory>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>

template <typename T>
struct Node {
    Node(const T& value, Node* left = nullptr, Node* right = nullptr)
        : val(value), pLeft(left), pRight(right) {}
    T     val;
    Node* pLeft  = nullptr;
    Node* pRight = nullptr;
};

template <typename T>
class BTree {
public:
    BTree(const std::string& file_name) {
        std::ifstream file(file_name);
        if (!file.is_open()) {
            std::cerr << "can not open the file: " << file_name << std::endl;
            return;
        }
        std::vector<T> v;
        std::vector<T> preOrder, inOrder;
        T              tmp;
        while (file >> tmp) {
            v.push_back(tmp);
        }
        preOrder.insert(preOrder.begin(), v.begin(), v.begin() + v.size() / 2);
        inOrder.insert(inOrder.begin(), v.begin() + v.size() / 2, v.end());
        mRoot = buildTree(preOrder, inOrder, 0, 0, preOrder.size());
    }

    void print(const std::string& mode) const {
        if (mode == "pre") preOrderPrint(mRoot);
        if (mode == "in") inOrderPrint(mRoot);
        if (mode == "post") postOrderPrint(mRoot);

        if (mode == "pre2") preOrderPrint2();
        if (mode == "in2") inOrderPrint2();
        if (mode == "post2") postOrderPrint2();

        std::cout << std::endl;
    }

private:
    Node<T>* mRoot;

    Node<T>* buildTree(std::vector<T>& preOrder, std::vector<T>& inOrder,
                       int p1, int p2, int n) {
        if (n == 0) return nullptr;
        if (n == 1) return new Node<T>(preOrder[p1], nullptr, nullptr);

        // find root in inorder sequence
        int i = 0;
        for (; preOrder[p1] != inOrder[p2 + i]; i++)
            ;
        Node<T>*leftTree, *rightTree;
        leftTree = buildTree(preOrder, inOrder, p1 + 1, p2, i),
        rightTree =
            buildTree(preOrder, inOrder, p1 + i + 1, p2 + i + 1, n - i - 1);
        return new Node<T>(preOrder[p1], leftTree, rightTree);
    }

    void preOrderPrint(Node<T>* root) const {
        if (root == nullptr) return;
        std::cout << root->val << " ";
        preOrderPrint(root->pLeft);
        preOrderPrint(root->pRight);
    }
    void inOrderPrint(Node<T>* root) const {
        if (root == nullptr) return;
        inOrderPrint(root->pLeft);
        std::cout << root->val << " ";
        inOrderPrint(root->pRight);
    }
    void postOrderPrint(Node<T>* root) const {
        if (root == nullptr) return;
        postOrderPrint(root->pLeft);
        postOrderPrint(root->pRight);
        std::cout << root->val << " ";
    }
    void preOrderPrint2() const {
        std::stack<const Node<T>*> s;
        const Node<T>*             p = mRoot;
        while (p || !s.empty()) {
            if (p) {
                std::cout << p->val << " ";
                s.push(p);
                p = p->pLeft;
            } else {
                p = s.top();
                s.pop();
                p = p->pRight;
            }
        }
    }
    void inOrderPrint2() const {
        std::stack<const Node<T>*> s;
        const Node<T>*             p = mRoot;
        while (p || !s.empty()) {
            if (p) {
                s.push(p);
                p = p->pLeft;
            } else {
                p = s.top();
                s.pop();
                std::cout << p->val << " ";
                p = p->pRight;
            }
        }
    }
    void postOrderPrint2() const {
        std::stack<const Node<T>*> s;
        const Node<T>*             p     = mRoot;
        const Node<T>*             pLast = nullptr;
        while (p) {
            s.push(p);
            p = p->pLeft;
        }
        while (!s.empty()) {
            p = s.top();
            s.pop();
            if (p->pRight == nullptr || p->pRight == pLast) {
                std::cout << p->val << " ";
                pLast = p;
            } else {
                s.push(p);
                p = p->pRight;
                while (p) {
                    s.push(p);
                    p = p->pLeft;
                }
            }
        }
    }
};
