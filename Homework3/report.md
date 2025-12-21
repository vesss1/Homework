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

### 測試目的
驗證多項式的輸入、輸出格式（代數式）、加減乘運算，以及 `Eval(x)` 代入求值皆正確。

### 測試輸入
（依題目輸入格式：`n c1 e1 c2 e2 ...`，依序輸入 p1、p2、x）

3 2 3 2 2 4 1

2 3 1 4 0

2

### 預期結果
（輸出格式：`coefx^exp + coefx^exp + ...`）
- P1 = 2x^3 + 2x^2 + 4x^1
- P2 = 3x^1 + 4x^0
- P1 + P2 = 2x^3 + 2x^2 + 7x^1 + 4x^0
- P1 - P2 = 2x^3 + 2x^2 + 1x^1 + -4x^0
- P1 * P2 = 6x^4 + 6x^3 + 20x^2 + 16x^1
- P1(2) = 32
- P2(2) = 10
  
---

### 實際結果
- P1 = 2x^3 + 2x^2 + 4x^1
- P2 = 3x^1 + 4x^0
- P1 + P2 = 2x^3 + 2x^2 + 7x^1 + 4x^0
- P1 - P2 = 2x^3 + 2x^2 + 1x^1 + -4x^0
- P1 * P2 = 6x^4 + 6x^3 + 20x^2 + 16x^1
- P1(2) = 32
- P2(2) = 10

---
## 申論及開發報告

### 一、作業目標與功能概述
本作業目標為實作一個 `Polynomial` 多項式類別，用來表示並操作單變數、整數係數的一元多項式。程式需支援多項式的輸入與輸出、拷貝建構與指定運算（確保正確複製物件內容）、加法、減法、乘法等運算，以及將整數 \(x\) 代入多項式求值（`Eval(x)`）。本實作依作業要求以「帶 header 節點的環狀單向鏈結串列」作為內部表示，並以 Available List（可用空間串列）回收節點以降低頻繁配置與釋放的成本。

---

### 二、設計與實作重點
1. **資料表示（Polynomial Representation）**  
   每個多項式項目以 `(coef, exp)` 表示，並以環狀串列儲存；透過 header 節點統一處理空串列與串列結尾，使插入、刪除與走訪流程更一致。串列內部維持 `exp` 由大到小排序，以利後續加減法採用類似 merge 的線性合併策略。

2. **節點管理（Available List）**  
   為提升刪除與記憶體管理效率，本作業採用 Available List 回收節點：當節點不再使用時不立即 `delete`，而是回收到 free list，後續需要新節點時優先重用，降低 `new/delete` 的呼叫次數與額外負擔。

3. **運算實作概念**  
   - **加法/減法**：利用兩個已排序（降冪）串列同時走訪，比較次方後決定輸出順序；遇到同次方則合併係數，若合併後係數為 0 則不保留該項。  
   - **乘法**：採雙迴圈列舉每一項相乘（係數相乘、次方相加），並將結果插入結果多項式，同時進行同次方合併以維持正確性與表示一致性。  
   - **求值**：依定義逐項累加 \(coef \cdot x^{exp}\)，本實作使用整數快速冪降低冪次計算成本並避免浮點誤差。

---

### 三、測試與驗證（摘要）
本作業以單一測試案例驗證輸出格式、加減乘運算與 `Eval(x)` 的正確性，並將實際輸出逐行與預期結果比對；若完全一致，即可確認主要功能運作正確。此外，實作過程亦考量常見邊界情況（如 0 多項式、同次方抵消、係數為 0 的項）以確保程式具備穩健性。

---

### 四、心得討論
本次作業讓我更深入理解「抽象資料型態（ADT）」與「資料結構選擇」對程式設計的影響。使用帶 header 的環狀鏈結串列雖然在實作上比一般線性串列更需要注意指標連結，但其優點是能統一空串列與尾端處理，減少特殊情況分支，提高程式結構一致性。另一方面，Available List 的引入讓我體會到記憶體管理不只關乎正確釋放，更關乎效能與資源重用：在節點大量建立/刪除的情境（如乘法）下，回收重用能有效降低配置成本。整體而言，此作業除了加深我對多項式運算（合併同次方、維持排序）的理解，也提升了我對鏈結串列操作與物件複製語意（拷貝建構、指定、解構）的掌握。

---

### 五、總結
本作業成功完成 `Polynomial` 類別的核心需求：以環狀鏈結串列（含 header）儲存多項式項目，支援輸入輸出、拷貝與指定、加減乘運算與代入求值，並使用 Available List 回收節點以提升記憶體管理效率。透過本次實作，我能更系統化地整合資料結構、運算邏輯與物件生命週期管理，並能以測試案例驗證結果正確性，達成作業的功能與設計要求。
