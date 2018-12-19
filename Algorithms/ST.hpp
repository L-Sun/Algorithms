#pragma once
#include <iostream>
#include <iomanip>

template <typename K, typename V>
struct Node {
public:
    Node(const K& key, const V& value, Node* prev = nullptr,
         Node* next = nullptr)
        : m_key(key), m_value(value), m_pPrev(prev), m_pNext(next) {}

    void del() {
        if (m_pPrev) {
            m_pPrev->m_pNext = m_pNext;
        }
        if (m_pNext) {
            m_pNext->m_pPrev = m_pPrev;
        }
        this->~Node();
    }

    K     m_key;
    V     m_value;
    Node* m_pPrev;
    Node* m_pNext;
};

template <class N>
void swap(N* n1, N* n2) {
    if (!n1 || !n2 || n1 == n2) {
        return;
    }
    //  a <-> b <-> c <-> ... <-> x <-> y <-> z
    // pn1    n1   nn1           pn2    n2   nn2
    N *pn1 = n1->m_pPrev, *nn1 = n1->m_pNext, *pn2 = n2->m_pPrev,
      *nn2 = n2->m_pNext;

    if (pn1) pn1->m_pNext = n2;
    if (nn2) nn2->m_pPrev = n1;
    if (nn1 == n2) {
        n1->m_pPrev = n2;
        n2->m_pNext = n1;
    } else {
        nn1->m_pPrev = n2;
        pn2->m_pNext = n1;
        n1->m_pPrev  = pn2;
        n2->m_pNext  = nn1;
    }
    n2->m_pPrev = pn1;
    n1->m_pNext = nn2;
}

template <typename K, typename V>
class ST {
public:
    ST() : m_pBegin(nullptr), m_size(0) {}
    V get(const K& key) const { return (*this)[key]; }

    void insert(K key, V value) {
        if (empty()) {
            m_pBegin = new Node<K, V>(key, value);
            m_pEnd   = m_pBegin;
            m_size++;
            return;
        }
        Node<K, V>* node = find(key);
        if (node) {
            node->m_value = value;
        } else {
            Node<K, V>* tmp = new Node<K, V>(key, value, m_pEnd, nullptr);
            m_pEnd->m_pNext = tmp;
            m_pEnd          = tmp;
            m_size++;
        }
    }

    void del(K key) {
        Node<K, V>* node = find(key);
        if (node) {
            node->del();
            m_size--;
        } else
            throw std::out_of_range("invalid key");
    }

    bool   empty() const { return m_size == 0; }
    size_t size() const { return m_size; }

    V& operator[](const K& key) {
        Node<K, V>* node = find(key);
        if (node)
            return node->m_value;
        else
            throw std::out_of_range("invalid key");
    }

    void print() {
        Node<K, V>* curr = m_pBegin;
        while (curr) {
            std::cout << std::setw(20) << std::right << curr->m_key << "  |  "
                      << std::left << curr->m_value << std::endl;
            curr = curr->m_pNext;
        }
    }
    Node<K, V>* find(const K& key) {
        Node<K, V>* curr = m_pBegin;
        while (curr) {
            if (curr->m_key == key) {
                move_node(curr);
                return curr;
            }
            curr = curr->m_pNext;
        }
        return nullptr;
    }

protected:
    Node<K, V>* m_pBegin;
    Node<K, V>* m_pEnd;

private:
    size_t m_size;

    virtual void move_node(Node<K, V>* node) {}
};

template <typename K, typename V>
class ForwardST : public ST<K, V> {
private:
    void move_node(Node<K, V>* node) {
        swap(this->m_pBegin, node);
        this->m_pBegin = node;
    }
};
