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

    ChainIterator& operator++() {
        current = current->link;
        return *this;
    }

    bool operator!=(const ChainIterator& rhs) const { return current != rhs.current; }
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
        header->link = header; 
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
        if (x.terms.Size() == 0) {
            os << 0;
            return os;
        }

        ChainNode<Term>* h = x.terms.HeaderNode();
        ChainNode<Term>* p = h->link;

        os << p->data.coef << "x^" << p->data.exp;
        p = p->link;

        while (p != h) {
            os << " + " << p->data.coef << "x^" << p->data.exp;
            p = p->link;
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
