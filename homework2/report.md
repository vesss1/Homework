# 41343127
---
## 作業二
---
### 解題說明  
設計一個能進行多項式加法的程式。
每個多項式由若干項 (係數*coef*, 次方*exp*) 組成 建立 *Term* 類別來儲存每一項的係數與次方。  
*Polynomial*由多個*Term*組成，並使用動態陣列儲存，確保能根據輸入項數擴充容量，當容量不足時自動倍增。   
使用` Add() `函式逐一比較兩多項式的指數：  
若指數相同，則係數相加。  
若某一方的指數較大，直接加入結果多項式。  
最後輸出合併後的多項式。   

---
### 解題想法

資料結構設計

- 使用 Term 類別 來儲存「係數（coef）」與「次方（exp）」兩個屬性。
- 使用 Polynomial 類別 管理多項式，由數個 Term 組成。
- 不使用 vector，改以 固定大小陣列 或 動態配置陣列（new/delete） 來儲存所有項目。

---
程式實作
---

以下為主要程式碼：
```cpp
#include <iostream>
#include <algorithm>
#include <math.h>
using namespace std;

class Polynomial;  // 前置宣告


class Term 
{
    friend class Polynomial;  
    friend ostream& operator<<(ostream&, const Polynomial&);
private:
    float coef; // 係數
    int exp;    // 次方
};


class Polynomial 
{
private:
    Term* termArray;         //陣列指標
    int capacity;            //陣列大小
    int terms;               //實際存取的項的大小
public:
    Polynomial();                      //建構子
    ~Polynomial();                     //解構子
    Polynomial(const Polynomial&);     // 拷貝建構子
    Polynomial& operator=(const Polynomial&);  // 指定運算子

    void newTerm(const float, const int);
    Polynomial Add(const Polynomial& b) const;
    friend istream& operator>>(istream&, Polynomial&);            //朋友輸入函式
    friend ostream& operator<<(ostream&, const Polynomial&);      //朋友輸出函式
};

//初始化
Polynomial::Polynomial() 
{
    capacity = 2;
    terms = 0;
    termArray = new Term[capacity];
}

//解構子刪除原本的記憶體
Polynomial::~Polynomial() 
{
    delete[] termArray;
}

// ===== 拷貝建構子 =====
Polynomial::Polynomial(const Polynomial& other) 
{
    capacity = other.capacity;
    terms = other.terms;
    termArray = new Term[capacity];
    copy(other.termArray, other.termArray + terms, termArray);
}

//透過this指標操作
Polynomial& Polynomial::operator=(const Polynomial& other) 
{
    if (this == &other) return *this;
    delete[] termArray;
    capacity = other.capacity;
    terms = other.terms;
    termArray = new Term[capacity];
    copy(other.termArray, other.termArray + terms, termArray);
    return *this;
}

//輸入的多項式傳進newTerm
void Polynomial::newTerm(const float theCoef, const int theExp) 
{
    if (theCoef == 0) return;
    if (terms == capacity) {
        capacity *= 2;
        Term* temp = new Term[capacity];
        copy(termArray, termArray + terms, temp);
        delete[] termArray;
        termArray = temp;
    }
    termArray[terms].coef = theCoef;
    termArray[terms].exp = theExp;
    terms++;
}

//輸入函式
istream& operator>>(istream& is, Polynomial& poly) 
{
    int n;
    is >> n;  // 輸入項數
    for (int i = 0; i < n; i++) 
    {
        float c;
        int e;
        is >> c >> e;
        poly.newTerm(c, e);
    }   
    return is;
}

//輸出函式
ostream& operator<<(ostream& os, const Polynomial& poly) 
{
    for (int i = 0; i < poly.terms; i++) 
    {
        if (i > 0 && poly.termArray[i].coef >= 0)
            os << "+";
        os << poly.termArray[i].coef << "X^" << poly.termArray[i].exp;
    }
    return os;
}

//加法函式
Polynomial Polynomial::Add(const Polynomial& b) const 
{
    Polynomial c;
    int aPos = 0, bPos = 0;
    while (aPos < terms && bPos < b.terms) 
    {
        if (termArray[aPos].exp == b.termArray[bPos].exp) 
        {
            float sum = termArray[aPos].coef + b.termArray[bPos].coef;
            if (sum != 0)
                c.newTerm(sum, termArray[aPos].exp);
            aPos++;
            bPos++;
        }
        else if (termArray[aPos].exp > b.termArray[bPos].exp) 
        {
            c.newTerm(termArray[aPos].coef, termArray[aPos].exp);
            aPos++;
        }
        else {
            c.newTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
            bPos++;
        }
    }
    while (aPos < terms)
        c.newTerm(termArray[aPos].coef, termArray[aPos++].exp);
    while (bPos < b.terms)
        c.newTerm(b.termArray[bPos].coef, b.termArray[bPos++].exp);

    return c;
}

int main() 
{
    Polynomial A, B, C;
    cin >> A;
    cin >> B;
    C = A.Add(B);
    cout << C << endl;
}
```

---
### 效能分析
---

## ⚙️ 效能分析 (Performance Analysis)

### 🧮 時間複雜度 (Time Complexity)

假設多項式 A、B 各有 `m` 與 `n` 個項：

- **合併過程：**  
  使用兩個指標 `aPos`、`bPos` 逐項比較次方大小，  
  每次比較都至少會消耗一項（來自 A 或 B），  
  因此整個加法的時間複雜度為：

  \[
  O(m + n)
  \]

- **插入新項 (newTerm)：**  
  若在輸入階段需要依次方排序插入，最差情況為每次比較插入點，  
  時間複雜度為 \( O(k^2) \)，其中 \( k \) 為多項式項數。  
  但若輸入本身已是降冪順序，可視為 \( O(k) \)。

✅ **總結：**
- 建立多項式：\( O(k) \sim O(k^2) \)  
- 加法運算：\( O(m + n) \)  
- 輸出結果：\( O(m + n) \)

---

### 💾 空間複雜度 (Space Complexity)

- 儲存多項式需要空間與項數成正比：  
  \[
  O(m + n)
  \]
- 在相加過程中會建立一個新的結果多項式，  
  其項數最多為 \( m + n \)，因此整體空間使用仍為：

  \[
  O(m + n)
  \]

---



### ⚖️ 效能結論

| 階段 | 平均時間複雜度 | 空間複雜度 | 說明 |
|:-----|:----------------|:-------------|:------|
| 多項式建立 | O(k) ~ O(k²) | O(k) | 視輸入是否排序 |
| 多項式相加 | O(m + n) | O(m + n) | 兩指標線性合併 |
| 結果輸出 | O(m + n) | O(1) | 單次遍歷輸出 |

✅ **整體效能表現良好，為線性時間的合併運算。**

---

