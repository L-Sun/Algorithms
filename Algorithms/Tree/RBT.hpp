
template <typename K, typename V>
struct Node {
    Node(const K& key, const V& val) : m_key(key), m_val(val) {}
    bool         m_bIsRed = false;
    K            m_key;
    V            m_val;
    unsigned int m_size;

    Node* m_pLeft  = nullptr;
    Node* m_pRight = nullptr;
};

template <typename K, typename V>
class RBT {
public:
    const V& find(const K& key) const {}
    void     insert(const K& key, const V& val) { insert(m_pRoot, key, val); }

private:
    Node<K, V>* m_pRoot = nullptr;

    Node<K, V>* insert(Node<K, V>* pNode, const K& key, const V& val);
    Node<K, V>* rotateLeft(Node<K, V>* pNode);
    Node<K, V>* rotateRight(Node<K, V>* pNode);
    void        flipColors(Node<K, V>* pNode);

    const Node<K, V>* find(const Node<K, V>* pNode, const K& key) const;
    bool              isRed(const Node<K, V>* pNode) const;
    unsigned int      size(const Node<K, V>* pNode) const;
};

template <typename K, typename V>
Node<K, V>* RBT<K, V>::insert(Node<K, V>* pNode, const K& key, const V& val) {
    if (!pNode) {
        return new Node<K, V>(key, val);
    }
    if (key < pNode->m_key)
        pNode->m_pLeft = insert(pNode->m_pLeft, key, val);
    else if (key > pNode->m_key)
        pNode->m_pRight = insert(pNode->m_pRight, key, val);
    else
        pNode->m_val = val;

    if (isRed(pNode->m_pRight) && !isRed(pNode->m_pLeft))
        pNode = rotateLeft(pNode);
    if (isRed(pNode->m_pLeft) && isRed(pNode->m_pLeft->m_pLeft))
        pNode = rotateRight(pNode);
    if (isRed(pNode->m_pLeft) && isRed(pNode->m_pRight)) flipColors(pNode);

    pNode->m_size = size(pNode->m_pLeft) + size(pNode->m_pRight) + 1;
    return pNode;
}

template <typename K, typename V>
const Node<K, V>* RBT<K, V>::find(const Node<K, V>* pNode, const K& key) const {
    if (pNode == nullptr) return nullptr;
    if (key < pNode->m_pLeft)
        return find(pNode->m_pLeft, key);
    else if (key > pNode->m_pRight)
        return find(pNode->m_pRight, key);
    else
        return pNode;
}

template <typename K, typename V>
Node<K, V>* RBT<K, V>::rotateLeft(Node<K, V>* pNode) {
    Node<K, V>* _pNode = pNode->m_pRight;
    _pNode->m_bIsRed   = pNode->m_bIsRed;
    pNode->m_pRight    = _pNode->m_pLeft;
    _pNode->m_pLeft    = pNode;
    pNode->m_bIsRed    = true;
    _pNode->m_size     = pNode->m_size;
    pNode->m_size      = size(pNode->m_pLeft) + size(pNode->m_pRight) + 1;
    return _pNode;
}

template <typename K, typename V>
Node<K, V>* RBT<K, V>::rotateRight(Node<K, V>* pNode) {
    Node<K, V>* _pNode = pNode->m_pRight;
    _pNode->m_bIsRed   = pNode->m_bIsRed;
    pNode->m_pLeft     = _pNode->m_pRight;
    _pNode->m_pRight   = pNode;
    pNode->m_bIsRed    = true;
    _pNode->m_size     = pNode->m_size;
    pNode->m_size      = size(pNode->m_pLeft) + size(pNode->m_pRight) + 1;
    return _pNode;
}

template <typename K, typename V>
void RBT<K, V>::flipColors(Node<K, V>* pNode) {
    pNode->m_bIsRed           = true;
    pNode->m_pLeft->m_bIsRed  = false;
    pNode->m_pRight->m_bIsRed = false;
}

template <typename K, typename V>
bool RBT<K, V>::isRed(const Node<K, V>* pNode) const {
    return pNode == nullptr ? false : (pNode->isRed);
}

template <typename K, typename V>
unsigned int RBT<K, V>::size(const Node<K, V>* pNode) const {
    return pNode == nullptr ? 0 : pNode->m_size;
}
