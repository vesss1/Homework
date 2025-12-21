# 41343127
---
### 作業三
---
### 解題說明
作業要求實作一個多項式類別，用來表示並操作單變數、整數係數的多項式。
程式需支援多項式的輸入與輸出、複製與指定、加減乘等基本運算，以及將特定數值代入多項式進行求值。

---
### 解題想法
先定義多項式的每一項為 (係數 coef, 次方 exp)，用一個帶 header 的環狀單向串列依 exp 由大到小儲存，確保同次方只會出現一次；讀入時逐項插入並在遇到相同 exp 時合併係數、係數變 0 就刪除。加減法利用兩個已排序串列做 merge：比較次方大小，次方相同就係數相加/相減；乘法用雙迴圈枚舉兩多項式所有項相乘（係數相乘、次方相加），再插回結果多項式並合併同次方。節點的配置與釋放則用 Available List 回收重用，刪除節點不直接 delete、需要新節點先重用回收節點，以符合作業的記憶體管理要求。

---
### 計劃實作
---

以下為主要程式碼：
```cpp
#include <iostream>
using namespace std;

template <class T>
class ChainNode {
public:
    T data;
    ChainNode<T>* link;
    ChainNode() : data(), link(nullptr) {}
    ChainNode(const T& d, ChainNode<T>* l = nullptr) : data(d), link(l) {}
};

template <class T>
class AvailableList {
private:
    static ChainNode<T>* avail;

public:
    static bool IsEmpty() { return avail == nullptr; }

    static ChainNode<T>* GetNode() {
        if (avail) {
            ChainNode<T>* node = avail;
            avail = avail->link;
            node->link = nullptr;
            return node;
        }
        return new ChainNode<T>();
    }

    static void ReturnNode(ChainNode<T>* node) {
        if (!node) return;
        node->link = avail;
        avail = node;
    }
};

template <class T>
ChainNode<T>* AvailableList<T>::avail = nullptr;

template <class T>
class ChainIterator {
private:
    ChainNode<T>* current;
    ChainNode<T>* header;

public:
    ChainIterator() : current(nullptr), header(nullptr) {}
    ChainIterator(ChainNode<T>* cur, ChainNode<T>* head) : current(cur), header(head) {}

    T& operator*() const { return current->data; }
    T* operator->() const { return &(current->data); }

    ChainIterator& operator++() {
        current = current->link;
        return *this;
    }

    ChainIterator operator++(int) {
        ChainIterator tmp = *this;
        current = current->link;
        return tmp;
    }

    bool operator==(const ChainIterator& rhs) const { return current == rhs.current; }
    bool operator!=(const ChainIterator& rhs) const { return current != rhs.current; }

    int operator-(const ChainIterator& rhs) const {
        if (rhs.header != header) return -1;
        int dist = 0;
        ChainNode<T>* p = rhs.current;
        while (p != current && p != header) {
            p = p->link;
            ++dist;
        }
        return (p == current) ? dist : -1;
    }
};

template <class T>
class Chain {
private:
    ChainNode<T>* header;
    ChainNode<T>* tail;
    int size;

public:
    using iterator = ChainIterator<T>;

    Chain() : header(new ChainNode<T>()), tail(nullptr), size(0) {
        header->link = header; // circular
        tail = header;
    }

    Chain(const Chain& other) : Chain() {
        for (auto it = other.Begin(); it != other.End(); ++it) {
            PushBack(*it);
        }
    }

    Chain& operator=(const Chain& rhs) {
        if (this == &rhs) return *this;
        Clear();
        for (auto it = rhs.Begin(); it != rhs.End(); ++it) {
            PushBack(*it);
        }
        return *this;
    }

    ~Chain() {
        Clear();
        delete header;
        header = nullptr;
        tail = nullptr;
    }

    bool IsEmpty() const { return size == 0; }
    int Size() const { return size; }

    iterator Begin() const { return iterator(header->link, header); }
    iterator End() const { return iterator(header, header); }

    ChainNode<T>* HeaderNode() const { return header; }

    void Clear() {
        ChainNode<T>* cur = header->link;
        while (cur != header) {
            ChainNode<T>* nxt = cur->link;
            AvailableList<T>::ReturnNode(cur);
            cur = nxt;
        }
        header->link = header;
        tail = header;
        size = 0;
    }

    ChainNode<T>* InsertAfter(ChainNode<T>* prev, const T& value) {
        ChainNode<T>* node = AvailableList<T>::GetNode();
        node->data = value;
        node->link = prev->link;
        prev->link = node;

        if (prev == tail) tail = node;
        if (node->link == header) tail = node;
        ++size;
        return node;
    }

    void EraseAfter(ChainNode<T>* prev) {
        ChainNode<T>* target = prev->link;
        if (!target || target == header) return;

        prev->link = target->link;
        if (target == tail) tail = prev;
        AvailableList<T>::ReturnNode(target);
        --size;
    }

    void PushBack(const T& value) {
        InsertAfter(tail, value);
    }
};

class Polynomial {
private:
    struct Term {
        int coef;
        int exp;
        Term(int c = 0, int e = 0) : coef(c), exp(e) {}
    };

    Chain<Term> terms;
    // 整數次方：x^e（e>=0）
    static long long PowInt(long long x, int e) {
        long long r = 1;
        while (e > 0) {
            if (e & 1) r *= x;
            x *= x;
            e >>= 1;
        }
        return r;
    }

    void AddTermSorted(int coef, int exp) {
        if (coef == 0) return;

        ChainNode<Term>* head = terms.HeaderNode();
        ChainNode<Term>* prev = head;
        ChainNode<Term>* cur  = head->link;
        while (cur != head && cur->data.exp > exp) {
            prev = cur;
            cur = cur->link;
        }

        if (cur != head && cur->data.exp == exp) {
            cur->data.coef += coef;
            if (cur->data.coef == 0) {
                terms.EraseAfter(prev);
            }
            return;
        }

        terms.InsertAfter(prev, Term(coef, exp));
    }

public:
    Polynomial() = default;
    Polynomial(const Polynomial& a) : terms(a.terms) {}

    const Polynomial& operator=(const Polynomial& a) {
        if (this == &a) return *this;
        terms = a.terms;
        return *this;
    }

    ~Polynomial() = default;

    Polynomial operator+(const Polynomial& b) const {
        Polynomial r;

        ChainNode<Term>* h1 = terms.HeaderNode();
        ChainNode<Term>* h2 = b.terms.HeaderNode();
        ChainNode<Term>* p = h1->link;
        ChainNode<Term>* q = h2->link;
        while (p != h1 && q != h2) {
            if (p->data.exp == q->data.exp) {
                int c = p->data.coef + q->data.coef;
                if (c != 0) r.terms.PushBack(Term(c, p->data.exp));
                p = p->link;
                q = q->link;
            } else if (p->data.exp > q->data.exp) {
                r.terms.PushBack(p->data);
                p = p->link;
            } else {
                r.terms.PushBack(q->data);
                q = q->link;
            }
        }
        while (p != h1) { r.terms.PushBack(p->data); p = p->link; }
        while (q != h2) { r.terms.PushBack(q->data); q = q->link; }

        return r;
    }

    Polynomial operator-(const Polynomial& b) const {
        Polynomial r;
        ChainNode<Term>* h1 = terms.HeaderNode();
        ChainNode<Term>* h2 = b.terms.HeaderNode();
        ChainNode<Term>* p = h1->link;
        ChainNode<Term>* q = h2->link;

        while (p != h1 && q != h2) {
            if (p->data.exp == q->data.exp) {
                int c = p->data.coef - q->data.coef;
                if (c != 0) r.terms.PushBack(Term(c, p->data.exp));
                p = p->link;
                q = q->link;
            } else if (p->data.exp > q->data.exp) {
                r.terms.PushBack(p->data);
                p = p->link;
            } else {
                r.terms.PushBack(Term(-q->data.coef, q->data.exp));
                q = q->link;
            }
        }
        while (p != h1) { r.terms.PushBack(p->data); p = p->link; }
        while (q != h2) { r.terms.PushBack(Term(-q->data.coef, q->data.exp)); q = q->link; }

        return r;
    }

    Polynomial operator*(const Polynomial& b) const {
        Polynomial r;

        ChainNode<Term>* h1 = terms.HeaderNode();
        ChainNode<Term>* h2 = b.terms.HeaderNode();

        for (ChainNode<Term>* p = h1->link; p != h1; p = p->link) {
            for (ChainNode<Term>* q = h2->link; q != h2; q = q->link) {
                long long c = 1LL * p->data.coef * q->data.coef;
                int e = p->data.exp + q->data.exp;
                if (c != 0) r.AddTermSorted((int)c, e);
            }
        }
        return r;
    }

    long long Eval(int x) const {
        long long sum = 0;
        ChainNode<Term>* h = terms.HeaderNode();
        for (ChainNode<Term>* p = h->link; p != h; p = p->link) {
            sum += 1LL * p->data.coef * PowInt((long long)x, p->data.exp);
        }
        return sum;
    }
    friend istream& operator>>(istream& is, Polynomial& x) {
        int n;
        if (!(is >> n)) return is;

        x.terms.Clear();

        for (int i = 0; i < n; ++i) {
            int c, e;
            is >> c >> e;
            x.AddTermSorted(c, e);
        }
        return is;
    }

    friend ostream& operator<<(ostream& os, const Polynomial& x) {
        os << x.terms.Size();
        ChainNode<Term>* h = x.terms.HeaderNode();
        for (ChainNode<Term>* p = h->link; p != h; p = p->link) {
            os << ' ' << p->data.coef << ' ' << p->data.exp;
        }
        return os;
    }
};

int main() {
    int x;
    Polynomial p1, p2;
    cin >> p1 >> p2;
    cin >> x;

    cout << "P1 = " << p1 << endl;
    cout << "P2 = " << p2 << endl;
    cout << "P1 + P2 = " << (p1 + p2) << endl;
    cout << "P1 - P2 = " << (p1 - p2) << endl;
    cout << "P1 * P2 = " << (p1 * p2) << endl;
    cout << "P1(" << x << ") = " << p1.Eval(x) << endl;
    cout << "P2(" << x << ") = " << p2.Eval(x) << endl;

    return 0;
}
```
