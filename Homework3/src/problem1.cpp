#include <iostream>
#include <cmath>

using std::istream;
using std::ostream;
using std::cout;
using std::cin;
using std::endl;

/*
  ================== ChainNode / AvailableList / ChainIterator / Chain ==================
  - Chain<T>：帶 header 節點的環狀單向鏈結串列
  - AvailableList<T>：可用空間串列（回收節點重用）
*/

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
    ChainNode<T>* header; // End == header（環狀串列）

public:
    ChainIterator() : current(nullptr), header(nullptr) {}
    ChainIterator(ChainNode<T>* cur, ChainNode<T>* head) : current(cur), header(head) {}

    T& operator*() const { return current->data; }
    T* operator->() const { return &(current->data); }

    ChainIterator& operator++() { // prefix
        current = current->link;
        return *this;
    }

    ChainIterator operator++(int) { // postfix
        ChainIterator tmp = *this;
        current = current->link;
        return tmp;
    }

    bool operator==(const ChainIterator& rhs) const { return current == rhs.current; }
    bool operator!=(const ChainIterator& rhs) const { return current != rhs.current; }

    // 距離：從 rhs 走到 this 的步數；不同串列回傳 -1
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

    ChainNode<T>* Node() const { return current; }
    ChainNode<T>* Header() const { return header; }
};

template <class T>
class Chain {
private:
    ChainNode<T>* header; // sentinel
    ChainNode<T>* tail;   // 最後一個實節點（空時 tail==header）
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
        Clear();       // 回收所有實節點到 AvailableList
        delete header; // header 不丟回 available list
        header = nullptr;
        tail = nullptr;
    }

    bool IsEmpty() const { return size == 0; }
    int Size() const { return size; }

    iterator Begin() const { return iterator(header->link, header); }
    iterator End() const { return iterator(header, header); }

    ChainNode<T>* HeaderNode() const { return header; }
    ChainNode<T>* TailNode() const { return tail; }

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

    // 在 prev 後插入新節點
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

    // 刪除 prev 後的節點（若不是 header）
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

/*
  ================================ Polynomial ================================
  - 內部用 Chain<Term>（環狀 + header）
  - Term = (coef, exp)
  - exp 由大到小排序；同次方合併；coef=0 刪除節點（回收至 AvailableList）
*/

class Polynomial {
private:
    struct Term {
        int coef;
        int exp;
        Term(int c = 0, int e = 0) : coef(c), exp(e) {}
    };

    Chain<Term> terms;

    // 插入或合併一項，保持 exp 降冪
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

    // Copy constructor
    Polynomial(const Polynomial& a) : terms(a.terms) {}

    // Assignment operator
    const Polynomial& operator=(const Polynomial& a) {
        if (this == &a) return *this;
        terms = a.terms;
        return *this;
    }

    // Destructor：由 Chain 解構子回收節點到 AvailableList
    ~Polynomial() = default;

    // Addition：兩個降冪串列 merge，同次方合併
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

    // Subtraction
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

    // Multiplication：雙迴圈產生項目 + 以 AddTermSorted 合併同次方
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

    // Evaluate：P(x)=Σ coef*x^exp
    float Evaluate(float x) const {
        float sum = 0.0f;
        ChainNode<Term>* h = terms.HeaderNode();
        for (ChainNode<Term>* p = h->link; p != h; p = p->link) {
            sum += (float)p->data.coef * (float)std::pow(x, (float)p->data.exp);
        }
        return sum;
    }

    // Input: n c1 e1 c2 e2 ... cn en
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

    // Output: n c1 e1 c2 e2 ... cn en
    friend ostream& operator<<(ostream& os, const Polynomial& x) {
        os << x.terms.Size();
        ChainNode<Term>* h = x.terms.HeaderNode();
        for (ChainNode<Term>* p = h->link; p != h; p = p->link) {
            os << ' ' << p->data.coef << ' ' << p->data.exp;
        }
        return os;
    }
};

/*
  ================================ Test Main ================================
  輸入格式（依題目）：n c1 e1 c2 e2 ... cn en
*/
int main() {
    Polynomial A, B;

    cout << "Input A as: n c1 e1 c2 e2 ... cn en\n";
    cin >> A;

    cout << "Input B as: n c1 e1 c2 e2 ... cn en\n";
    cin >> B;

    cout << "\nA = " << A << endl;
    cout << "B = " << B << endl;

    Polynomial S = A + B;
    Polynomial D = A - B;
    Polynomial M = A * B;

    cout << "\nA + B = " << S << endl;
    cout << "A - B = " << D << endl;
    cout << "A * B = " << M << endl;

    float x;
    cout << "\nInput x for Evaluate(A, x): ";
    cin >> x;
    cout << "A(" << x << ") = " << A.Evaluate(x) << endl;

    return 0;
}

