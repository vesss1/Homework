# 41343127
---
### 作業4

## **Max/Min Heap & Binary Search Tree**

**解題說明**
這題的重點是完成 heap 的基本功能，分別做出 MinHeap 和 MaxHeap。

在 MinHeap 中，最小的元素會放在最上面；在 MaxHeap 中，最大的元素會放在最上面。
程式在插入資料時，會先把新資料放到最後面，再往上比較與交換，讓整個結構維持正確。
刪除頂端元素時，會先把最後一個元素補到最前面，再往下調整，讓 heap 回到原本應有的規則。

這題實作的功能包含：
* 判斷 heap 是否為空
* 取得頂端元素
* 插入新元素
* 刪除頂端元素

---

### 程式設計  
```cpp
//41343127
#include <iostream>
#include <string>
using namespace std;

template <class T>
class Heap {
private:
    T* h;
    int size;
    int capacity;
    bool isMin;

public:// 建構子
    Heap(int c, bool m) {
        capacity = c;
        size = 0;
        isMin = m;
        h = new T[capacity];
    }

    ~Heap() {// 解構子
        delete[] h;
    }

    bool IsEmpty() const {
        if (size == 0) {
            return true;
        }
        else {
            return false;
        }
    }

    T Top() const {// MinHeap 回傳最小值，MaxHeap 回傳最大值
        return h[0];
    }

    void Push(T x) {// 插入新元素
        if (size >= capacity) {
            return;
        }

        h[size] = x;// 先把新元素放到最後面
        int i = size;
        size++;

        while (i > 0) {
            int p = (i - 1) / 2;// 父節點位置

            if (isMin) {// MinHeap：如果子節點比父節點小，就交換
                if (h[i] < h[p]) {
                    T temp = h[i];
                    h[i] = h[p];
                    h[p] = temp;
                    i = p;
                }
                else {
                    break;
                }
            }
            else {// MaxHeap：如果子節點比父節點大，就交換
                if (h[i] > h[p]) {
                    T temp = h[i];
                    h[i] = h[p];
                    h[p] = temp;
                    i = p;
                }
                else {
                    break;
                }
            }
        }
    }
  // 刪除頂端元素  :<
    void Pop() {
        if (size == 0) {
            return;
        }

        h[0] = h[size - 1];
        size--;

        int i = 0;

        while (2 * i + 1 < size) {
            int l = 2 * i + 1;
            int r = 2 * i + 2;
            int t = l;

            if (isMin) {
                if (r < size && h[r] < h[l]) {
                    t = r;
                }

                if (h[t] < h[i]) {
                    T temp = h[i];
                    h[i] = h[t];
                    h[t] = temp;
                    i = t;
                }
                else {
                    break;
                }
            }
            else {
                if (r < size && h[r] > h[l]) {
                    t = r;
                }

                if (h[t] > h[i]) {
                    T temp = h[i];
                    h[i] = h[t];
                    h[t] = temp;
                    i = t;//:<
                }
                else {
                    break;
                }
            }
        }
    }
};

int main() {
    int n, x;
    cin >> n;

    Heap<int> minHeap(n, true);
    Heap<int> maxHeap(n, false);

    for (int i = 0; i < n; i++) {
        cin >> x;
        minHeap.Push(x);
        maxHeap.Push(x);//41343127
    }

    cout << "MinHeap" << endl;
    cout << "IsEmpty: " << minHeap.IsEmpty() << endl;
    cout << "Top: " << minHeap.Top() << endl;
    minHeap.Pop();
    cout << "After Pop, Top: " << minHeap.Top() << endl;

    cout << "MaxHeap" << endl;//f
    cout << "IsEmpty: " << maxHeap.IsEmpty() << endl;
    cout << "Top: " << maxHeap.Top() << endl;
    maxHeap.Pop();
    cout << "After Pop, Top: " << maxHeap.Top() << endl;

    return 0;
}
```

---
### 效能分析
**時間複雜度：** `O(1)`
---

### `Push()`
插入資料時，先把新元素放到陣列最後面，然後往上調整到正確位置。

在最差情況下，新元素可能一路從最底層換到最上層。  
heap 的高度大約是 `log n`，所以最多調整 `log n` 次。

**時間複雜度：** `O(log n)`

---

### `Pop()`
刪除頂端元素時，先把最後一個元素補到根節點，再往下調整。

在最差情況下，這個元素可能一路往下移到最底層，最多也是調整 `log n` 次。

**時間複雜度：** `O(log n)`

---

## 整體分析
heap 的優點是可以快速取得最小值或最大值：

- 取得頂端元素很快：`O(1)`
- 插入與刪除也不會太慢：`O(log n)`

所以 heap 很適合拿來做優先佇列這類需要常常取出最大值或最小值的情況。

---
---

### `Top()`
直接回傳陣列最前面的元素，也就是 heap 的根節點。

- MinHeap 回傳最小值
- MaxHeap 回傳最大值

不需要比較其他元素。

**時間複雜度：** `O(1)`

### 測試與驗證
這題的測試方式，主要是確認 MinHeap 和 MaxHeap 是否都能正常運作。

測試時先輸入一組整數資料，並把同一組資料同時放進 MinHeap 和 MaxHeap。

| 測試案例 | 組數 | 測資 | MinHeap Tree | MaxHeap Tree |
|---|---:|---|---|---|
| 測試一 | 5 | 9 3 7 1 6 | Level 0: 1<br>Level 1: 3 7<br>Level 2: 9 6 | Level 0: 9<br>Level 1: 6 7<br>Level 2: 1 3 |
| 測試二 | 7 | -1 44 -5 100 51 -101 8 | Level 0: -101<br>Level 1: 44 -5<br>Level 2: 100 51 -1<br>Level 3: 8 | Level 0: 100<br>Level 1: 51 8<br>Level 2: -1 44 -101<br>Level 3: -5 |

---

## 結論

這題成功完成了 MinHeap 和 MaxHeap 的基本功能，包括插入、取得頂端元素、判斷是否為空，以及刪除頂端元素。

從測試結果可以看出，程式能正確維持 heap 的規則。
MinHeap 可以正確保留最小值在最上面，MaxHeap 可以正確保留最大值在最上面，而且刪除頂端元素後也能正常重新調整。

整體來說，這份程式已經完成題目要求的功能，也讓我更了解 heap 的運作方式。

---

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#  Binary Search Tree

## 解題說明

第 2 題分成兩個部分。

### (a)

這一小題是要觀察 Binary Search Tree 在隨機插入資料時，高度和 `log2(n)` 的關係。

程式先建立一棵空的 BST，然後對不同的 `n` 值進行隨機插入。  
每次插入完成後，計算該樹的高度 `height`，再算出：

\[
\frac{\text{height}}{\log_2 n}
\]

最後把不同 `n` 的結果列出，方便後續整理成圖表。  
這題的重點不是樹的外觀，而是要觀察這個比值是否大致接近某個常數。

### (b)

這一小題是要實作 BST 的刪除功能，也就是刪除鍵值為 `k` 的節點。

刪除節點時，會遇到三種情況：

1. 該節點沒有子節點，直接刪除
2. 該節點只有一個子節點，用子節點取代它
3. 該節點有兩個子節點，找右子樹中最小的節點來取代，再刪掉那個替代節點

這樣做可以在刪除節點之後，仍然維持 BST 原本的性質。

---

## 心得與問題


---

## 申論及開發報告


在資料儲存方面，程式使用線性容器來保存 heap 的內容，並利用索引的方式計算父節點與子節點的位置。這樣可以不需要真的建立樹的節點結構，就能完成 heap 的功能，也讓插入與刪除的操作比較容易實作。

在測試部分，程式分別建立 MinHeap 與 MaxHeap，輸入同樣的整數資料後，觀察輸出的分層結果是否符合 heap 的規則。之後再測試頂端元素的取得，以及刪除頂端元素後的結果是否仍然正確。從結果來看，程式可以正常完成題目要求的內容。

整體來說，這次開發重點在於 heap 的調整流程是否正確，尤其是插入後的往上調整與刪除後的往下調整。完成這次作業後，我對 heap 的基本原理與實作方式都有更進一步的認識。
