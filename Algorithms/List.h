#include <memory>
#include <iostream>

namespace MyAlgorithms {
template <typename T>
struct Node {
    Node(T val, std::shared_ptr<Node> next = nullptr) : val(val), next(next) {}
    T                     val;
    std::shared_ptr<Node> next = nullptr;
};

template <typename T>
class List {
public:
    List() {}

    template <int D>
    List(T (&a)[D]) {
        pHead  = std::make_shared<Node<T>>(a[0]);
        auto p = pHead;
        for (size_t i = 1; i < D; i++) {
            insert(p, a[i]);
            p = p->next;
        }
    }

    List(std::shared_ptr<Node<T>> head) : pHead(head) {
        int len = 0;
        while (head) {
            head = head->next;
            len++;
        }
        length = len;
    }

    ~List() {
        // 直接滞空头结点，利用智能指针的特性，析构后继结点
        pHead = nullptr;
    }

    void insert(std::shared_ptr<Node<T>> p, T val) {
        auto q  = std::make_shared<Node<T>>(val, p->next);
        p->next = q;
        length++;
    }
    void del(std::shared_ptr<Node<T>> p) {
        if (p->next == nullptr) {
            if (p == pHead) {
                pHead  = nullptr;
                length = 0;
                return;
            }
            auto q = pHead;
            // 找到尾元素的前继
            while (q->next != p) {
                q = q->next;
            }
            q->next = nullptr;
        } else {
            auto q = p->next;
            std::swap(p->val, q->val);
            p->next = q->next;
        }
        length--;
    }

    std::shared_ptr<Node<T>> find(const T&                 x,
                                  std::shared_ptr<Node<T>> head = nullptr) {
        auto p = head ? head : pHead;
        while (p) {
            if (p->val == x) break;
            p = p->next;
        }
        return p;
    }

    void del(const T& x) { del(x, pHead); }

    void reserve() {
        auto end = reserve(pHead, nullptr);
        pHead    = end;
    }

    void reserve_1() {
        if (pHead->next == nullptr) return;
        std::shared_ptr<Node<T>> p1 = nullptr, p2 = pHead, p3 = pHead->next;
        while (true) {
            p2->next = p1;
            p1       = p2;
            p2       = p3;
            if (p2 == nullptr) break;
            p3 = p3->next;
        }
        pHead = p1;  // 更新头节点
    }

    void sort() { sort(pHead); }

    void delBetween(const T& l, const T& r) {
        std::shared_ptr<Node<T>> p1 = pHead;
        std::shared_ptr<Node<T>> p2 = pHead->next;
        if (p1->val > l && p1->val < r) {
            p1 = p1->next;
            length--;
        }
        while (p2) {
            if (p2->val > l && p2->val < r) {
                p1->next = p2->next;
                p2       = p2->next;
                length--;
            } else {
                p1 = p1->next;
                p2 = p2->next;
            }
        }
    }

    void sortAndDelReapte() {
        sort(pHead);
        if (pHead->next == nullptr) return;
        auto p1 = pHead, p2 = pHead->next;

        while (p2) {
            if (p1->val == p2->val) {
                p1->next = p2->next;
            } else
                p1 = p1->next;
            p2 = p1->next;
        }
    }

    operator std::shared_ptr<Node<T>>&() { return pHead; }

    friend std::ostream& operator<<(std::ostream& out, const List& l) {
        auto p = l.pHead;
        while (p->next) {
            out << p->val << " -> ";
            p = p->next;
        }
        return out << p->val;
    }

private:
    std::shared_ptr<Node<T>> pHead;
    size_t                   length = 0;

    // 原地删除
    void del(const T& x, std::shared_ptr<Node<T>>& head) {
        if (head == nullptr) return;
        if (head->val == x) {
            head = head->next;
            length--;
            del(x, head);
        } else
            del(x, head->next);
    }

    std::shared_ptr<Node<T>> reserve(std::shared_ptr<Node<T>>& p,
                                     std::shared_ptr<Node<T>>  prev_node) {
        auto end = p;
        if (p->next) {
            end = reserve(p->next, p);
        }
        p->next = prev_node;
        return end;
    }

    void sort(std::shared_ptr<Node<T>> head) {
        if (head->next == nullptr) return;
        auto min_p = head, p = head;
        while (p) {
            if (p->val < min_p->val) min_p = p;
            p = p->next;
        }
        std::swap(head->val, min_p->val);
        sort(head->next);
    }
};

void extractEven(std::shared_ptr<Node<int>>& A, std::shared_ptr<Node<int>>& B) {
    if (A == nullptr) return;
    if (A->val % 2 == 0) {
        B = A;
        A = A->next;
        extractEven(A, B->next);
    } else {
        extractEven(A->next, B);
    }
}

void discreate(std::shared_ptr<Node<int>>& A, std::shared_ptr<Node<int>>& B) {
    std::shared_ptr<Node<int>> p     = A->next;
    std::shared_ptr<Node<int>> pre_p = A;
    std::shared_ptr<Node<int>> q     = nullptr;

    bool isEven = false;
    while (p) {
        if (isEven) {
            pre_p->next = p->next;
            p->next     = q;
            q           = p;
        } else {
            isEven = !isEven;
        }
        pre_p = pre_p->next;
        if (pre_p == nullptr) break;
        p = pre_p->next;
    }
    B = q;
}

template <typename T>
List<T> mergeInverse(List<T>& l1, List<T>& l2) {
    std::shared_ptr<Node<T>> A(l1);
    std::shared_ptr<Node<T>> B(l2);
    std::shared_ptr<Node<T>> C    = nullptr;
    std::shared_ptr<Node<T>> temp = nullptr;
    while (A && B) {
        if (A->val < B->val) {
            temp = A;
            A    = A->next;
        } else {
            temp = B;
            B    = B->next;
        }
        temp->next = C;
        C          = temp;
    }
    std::shared_ptr<Node<T>> p = A ? A : B;
    while (p) {
        temp       = p;
        p          = p->next;
        temp->next = C;
        C          = temp;
    }
    return List<T>(C);
}

template <typename T>
List<T> getCommon(List<T>& l1, List<T>& l2) {
    std::shared_ptr<Node<T>> ret;
    std::shared_ptr<Node<T>> A(l1);
    std::shared_ptr<Node<T>> B(l2);
    std::shared_ptr<Node<T>> end;
    while (A && B) {
        if (A->val < B->val)
            A = A->next;
        else if (A->val > B->val)
            B = B->next;
        else {
            if (ret == nullptr) {
                ret = std::make_shared<Node<T>>(A->val);
                end = ret;
            } else if (end->val != A->val) {
                end->next = std::make_shared<Node<T>>(A->val);
                end       = end->next;
            }
            A = A->next;
            B = B->next;
        }
    }
    return List<T>(ret);
}

}  // namespace MyAlgorithms
