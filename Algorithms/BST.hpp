#pragma once
#include <iostream>
#include <iomanip>
#include <memory>
#include <cassert>
#include <random>

template <typename K, typename V>
class BST;

template <typename K, typename V>
class Node {
    friend BST<K, V>;

public:
    Node() = default;
    Node(const K& key, const V& value) : m_key(key), m_val(value), m_size(1) {}

    void setVal(const V& value) { m_val = value; }

    K      getKey() const { return m_key; }
    V      getVal() const { return m_val; }
    size_t getSize() const { return m_size; }

    std::shared_ptr<Node> getLeft() const { return m_pLeft; }
    std::shared_ptr<Node> getRight() const { return m_pRight; }

private:
    K      m_key;
    V      m_val;
    size_t m_size = 0;

    std::shared_ptr<Node> m_pLeft;
    std::shared_ptr<Node> m_pRight;
};

template <typename K, typename V>
class BST {
    typedef Node<K, V> t_node;

public:
    V get(const K& key) const {
        std::shared_ptr<t_node> tmp = get(m_pRoot, key);
        return tmp ? tmp->m_val : 0;
    }
    K      min() const { return min(m_pRoot)->m_key; }
    K      max() const { return max(m_pRoot)->m_key; }
    K      floor(const K& key) const { return floor(m_pRoot, key)->m_key; }
    K      ceiling(const K& key) const { return ceiling(m_pRoot, key)->m_key; }
    K      select(size_t k) const { return select(m_pRoot, k)->m_key; }
    size_t rank(const K& key) const { return rank(m_pRoot, key); }
    size_t size() const { return size(m_pRoot); }
    size_t innerPathLen() const { return innerPathLen(m_pRoot); }
    size_t meanInnerPathLen() const { return innerPathLen() / size() + 1; }
    void   vertify() const { vertify(m_pRoot); }
    void   print(std::ostream& os) const { print(os, m_pRoot, ""); }

    void insert(const K& key, const V& value) {
        m_pRoot = insert(m_pRoot, key, value);
    }
    void delMin() { m_pRoot = delMin(m_pRoot); }
    void delMax() { m_pRoot = delMax(m_pRoot); }
    void del(const K& key, bool isRandom = true) {
        m_pRoot = del(m_pRoot, key, isRandom);
    }

private:
    std::shared_ptr<t_node> m_pRoot;

    std::shared_ptr<t_node> get(std::shared_ptr<t_node> node,
                                const K&                key) const {
        if (!node) return nullptr;
        if (key == node->m_key)
            return node;
        else if (key < node->m_key)
            return get(node->m_pLeft, key);
        else
            return get(node->m_pRight, key);
    }
    std::shared_ptr<t_node> min(std::shared_ptr<t_node> node) const {
        return node->m_pLeft ? min(node->m_pLeft) : node;
    }
    std::shared_ptr<t_node> max(std::shared_ptr<t_node> node) const {
        return node->m_pRight ? max(node->m_pRight) : node;
    }
    std::shared_ptr<t_node> floor(std::shared_ptr<t_node> node,
                                  const K&                key) const {
        // max(node->m_key) <= key
        if (!node) return nullptr;
        if (key == node->m_key) return node;
        if (key < node->m_key) return floor(node->m_pLeft, key);
        std::shared_ptr<t_node> tmp = floor(node->m_pRight, key);
        return tmp ? tmp : node;
    }
    std::shared_ptr<t_node> ceiling(std::shared_ptr<t_node> node,
                                    const K&                key) const {
        // min(node->m_key) >= key
        if (!node) return nullptr;
        if (key == node->m_key) return node;
        if (key > node->m_key) return ceiling(node->m_pRight, key);
        std::shared_ptr<t_node> tmp = ceiling(node->m_pLeft, key);
        return tmp ? tmp : node;
    }
    std::shared_ptr<t_node> select(std::shared_ptr<t_node> node,
                                   size_t                  k) const {
        if (!node) return nullptr;
        size_t sz = size(node->m_pLeft);
        if (k < sz)
            return select(node->m_pLeft, k);
        else if (k > sz)
            return select(node->m_pRight, k - sz - 1);
        else
            return node;
    }
    bool isLeaf(std::shared_ptr<t_node> node) const {
        return node->m_pLeft && node->m_pRight;
    }
    size_t rank(std::shared_ptr<t_node> node, const K& key) const {
        if (!node) return 0;
        if (key < node->m_key)
            return rank(node->m_pLeft, key);
        else if (key > node->m_key)
            return rank(node->m_pRight, key) + 1 + size(node->m_pLeft);
        else
            return size(node->m_pLeft);
    }
    size_t size(std::shared_ptr<t_node> node) const {
        return node ? node->m_size : 0;
    }
    size_t innerPathLen(std::shared_ptr<t_node> node) const {
        if (!node) return 0;
        size_t left  = node->m_pLeft ? innerPathLen(node->m_pLeft) : 0;
        size_t right = node->m_pRight ? innerPathLen(node->m_pRight) : 0;
        return size(node) - 1 + left + right;
    }
    size_t vertify(std::shared_ptr<t_node> node) const {
        if (!node) return 0;
        size_t v_sz = vertify(node->m_pLeft) + vertify(node->m_pRight) + 1;
        assert(v_sz == node->m_size);
        if (node->m_pLeft) {
            assert(node->m_key > node->m_pLeft->m_key);
            assert(node->m_pLeft.use_count() == 1);
        }
        if (node->m_pRight) {
            assert(node->m_key < node->m_pRight->m_key);
            assert(node->m_pRight.use_count() == 1);
        }
        return v_sz;
    }

    void print(std::ostream& os, std::shared_ptr<t_node> node,
               const std::string& prefix, bool isLeft = false) const {
        os << prefix;
        if (node) {
            os << (isLeft ? "├──" : "└──");
            // print the value of the node
            os << node->m_key << ":" << node->m_val << std::endl;

            // enter the next tree level - left and right branch
            if (node->m_pLeft || node->m_pRight) {
                print(os, node->m_pLeft, prefix + (isLeft ? "│  " : "   "),
                      true);
                print(os, node->m_pRight, prefix + (isLeft ? "│  " : "   "),
                      false);
            }
        } else {
            // if left or right node is empty
            os << (isLeft ? "├─" : "└─") << std::endl;
        }
    }

    std::shared_ptr<t_node> insert(std::shared_ptr<t_node> node, const K& key,
                                   const V& value) {
        if (!node) return std::make_shared<t_node>(key, value);

        if (key == node->m_key)
            node->m_val = value;
        else if (key < node->m_key)
            node->m_pLeft = insert(node->m_pLeft, key, value);
        else
            node->m_pRight = insert(node->m_pRight, key, value);
        node->m_size = size(node->m_pLeft) + size(node->m_pRight) + 1;
        return node;
    }
    std::shared_ptr<t_node> delMin(std::shared_ptr<t_node> node) {
        if (!node) return nullptr;
        if (!node->m_pLeft) return node->m_pRight;
        node->m_pLeft = delMin(node->m_pLeft);
        node->m_size  = size(node->m_pLeft) + size(node->m_pRight) + 1;
        return node;
    }
    std::shared_ptr<t_node> delMax(std::shared_ptr<t_node> node) {
        if (!node) return nullptr;
        if (!node->m_pRight) return node->m_pLeft;
        node->m_pRight = delMax(node->m_pRight);
        node->m_size   = size(node->m_pRight) + size(node->m_pLeft) + 1;
        return node;
    }
    std::shared_ptr<t_node> del(std::shared_ptr<t_node> node, const K& key,
                                bool isRandom) {
        if (!node) return nullptr;
        if (key < node->m_key)
            node->m_pLeft = del(node->m_pLeft, key, isRandom);
        else if (key > node->m_key)
            node->m_pRight = del(node->m_pRight, key, isRandom);
        else {
            if (!node->m_pLeft) return node->m_pRight;
            if (!node->m_pRight) return node->m_pLeft;
            auto tmp        = node;
            bool front_mode = false;
            if (isRandom) {
                std::random_device                 r;
                std::default_random_engine         e(r());
                std::uniform_int_distribution<int> dist(1, 10);
                front_mode = dist(e) % 2 == 0 ? true : false;
            }
            if (front_mode) {
                node           = max(tmp->m_pLeft);
                node->m_pLeft  = delMax(tmp->m_pLeft);
                node->m_pRight = tmp->m_pRight;

            } else {
                node           = min(tmp->m_pRight);
                node->m_pRight = delMin(tmp->m_pRight);
                node->m_pLeft  = tmp->m_pLeft;
            }
        }
        node->m_size = size(node->m_pLeft) + size(node->m_pRight) + 1;
        return node;
    }
};