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
```

---
# 效能分析
## 時間複雜度（Time Complexity）

令 \(P_1\) 有 \(m\) 項、\(P_2\) 有 \(n\) 項，最大次方 \(E=\max(\text{exp})\)，乘法結果合併後項數為 \(k\)（\(k \le mn\)）。

- **輸入 `>>`**：每插入一項需線性找位置（`AddTermSorted`），最壞  
  - \(P_1:\;O(m^2)\)，\(P_2:\;O(n^2)\)
- **輸出 `<<`**：走訪所有節點一次  
  - \(P_1:\;O(m)\)，\(P_2:\;O(n)\)
- **加法 `+` / 減法 `-`**：兩個降冪串列 merge，各走一次  
  - \(O(m+n)\)
- **乘法 `*`**：產生 \(mn\) 個乘積項；每次插入結果最壞需掃描 \(k\) 項  
  - 最壞 \(O(mn\cdot k)\)，且 \(k\approx mn\) 時為 **\(O(m^2n^2)\)**
- **代入 `Eval(x)`**：走訪 \(m\) 項；每項用快速冪 \(O(\log \text{exp})\)  
  - 上界 **\(O(m\log E)\)**

---

## 空間複雜度（Space Complexity）

- **多項式儲存**：每一項一個節點（另有 header 為常數）  
  - \(P_1:\;O(m)\)，\(P_2:\;O(n)\)
- **加法/減法結果**：結果最多約 \(m+n\) 項  
  - **\(O(m+n)\)**
- **乘法結果**：結果最多 \(k\) 項（\(k \le mn\)）  
  - **\(O(k)\subseteq O(mn)\)**
- **`Eval(x)`**：只用常數額外變數  
  - **\(O(1)\)**
- **Available List（回收節點）**：可能暫存回收但未重用的節點，最壞為線性量級，**不改變**整體大 O 階。
  
---
## 結果結論

- **時間複雜度**
  - 讀入：\(O(m^2)\)、\(O(n^2)\)
  - 加/減：\(O(m+n)\)
  - 乘：最壞 \(O(m^2n^2)\)
  - 代入求值：\(O(m\log E)\)

- **空間複雜度**
  - 儲存 \(P_1, P_2\)：\(O(m)+O(n)\)
  - 加/減結果：\(O(m+n)\)
  - 乘法結果：\(O(k)\subseteq O(mn)\)
  - `Eval(x)` 額外空間：\(O(1)\)
  - 
---
## 測試與驗證

### 測試目標
- 驗證 `>>`/`<<` 輸入輸出格式正確，且項目以 `exp` 降冪輸出。
- 驗證 `+`、`-`、`*` 運算結果正確（同次方合併、係數為 0 的項不輸出）。
- 驗證 `Eval(x)` 代入求值正確（整數運算）。
- 驗證特殊情況：空多項式、單項、多項抵消、含 0 係數輸入。

---

### 測試方法
- 以主程式既定流程輸入 `p1`、`p2`、`x`，比對輸出是否符合手算結果。
- 針對不同邊界情況設計測資（如下），逐一驗證輸出。

---

### 測試案例 1：一般情況（含同次方合併）
**輸入**
3 3 2 2 1 1 0
3 1 3 -2 1 4 0
2

**預期（手算重點）**
- \(P1 = 3x^2 + 2x + 1\)
- \(P2 = x^3 - 2x + 4\)
- \(P1+P2 = x^3 + 3x^2 + 5\)
- \(P1-P2 = -x^3 + 3x^2 + 4x - 3\)
- \(P1\cdot P2\) 依分配律展開並合併同次方
- \(P1(2)=3\cdot4+2\cdot2+1=17\)，\(P2(2)=8-4+4=8\)

---

### 測試案例 2：加法完全抵消（結果為 0 多項式）
**輸入**
2 5 3 -7 1
2 -5 3 7 1
3

**驗證點**
- `P1 + P2` 應輸出 `0`（表示 0 項，只有項數）。
- `P1 - P2` 應為 `2 10 3 -14 1`
- `Eval(3)` 可手算比對。

---

### 測試案例 3：其中一個為空多項式
**輸入**
0
2 4 2 1 0
5

**驗證點**
- `P1 = 0`
- `P1 + P2` 應等於 `P2`
- `P1 * P2` 應為 `0`
- `P1(5)` 應為 `0`

---

### 測試案例 4：乘法同次方大量合併
**輸入**
2 1 1 1 0
2 1 1 -1 0
10

**驗證點**
- \(P1 = x+1\)，\(P2 = x-1\)
- `P1 * P2 = x^2 - 1`（中間項 \(+x\) 與 \(-x\) 需合併抵消）
- `P1(10)=11`，`P2(10)=9`

---

### 測試案例 5：輸入含 0 係數（應忽略或合併後刪除）
**輸入**
3 0 5 2 2 0 0
1 3 1
2

**驗證點**
- `P1` 最終只應保留 `2x^2`
- 確認輸出項數與內容正確。

---

### 驗證結果判準
- 輸出格式完全符合主程式指定字串與 `operator<<` 格式。
- 各運算結果與手算一致，且：
  - `exp` 仍為降冪
  - 不存在係數為 0 的項
  - 同次方不會重複出現
- `Eval(x)` 結果與手算一致。

---
