#include <iostream>
#include <iomanip>
#include <memory>

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
    BST() {}
    ~BST() {}
    V    get(const K& key) const { return get(m_pRoot, key); }
    void insert(const K& key, const V& value) {
        m_pRoot = insert(m_pRoot, key, value);
    }
    size_t size() { return size(m_pRoot); }

    void print() { print(m_pRoot); }

private:
    std::shared_ptr<t_node> m_pRoot;

    V get(std::shared_ptr<t_node> node, const K& key) const {
        if (!node) return 0;
        if (node->m_key == key)
            return node->m_val;
        else if (node->m_key < key)
            return get(node->m_pLeft, key);
        else
            return get(node->m_pRight, key);
    }

    std::shared_ptr<t_node> insert(std::shared_ptr<t_node> node, const K& key,
                                   const V& value) {
        if (!node) return std::make_shared<t_node>(key, value);

        auto& left  = node->m_pLeft;
        auto& right = node->m_pRight;
        if (key == node->m_key)
            node->m_val = value;
        else if (key < node->m_key)
            left = insert(left, key, value);
        else
            right = insert(right, key, value);
        node->m_size = size(left) + size(right) + 1;
        return node;
    }
    size_t size(std::shared_ptr<t_node> node) {
        return node ? node->m_size : 0;
    }
    void print(std::shared_ptr<t_node> node) {
        if (!node) return;
        print(node->m_pLeft);
        std::cout << std::setw(15) << std::right << node->m_key << " | "
                  << std::left << node->m_val << std::endl;
        print(node->m_pRight);
    }
};