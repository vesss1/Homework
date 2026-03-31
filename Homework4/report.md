# 41343127
---
### 作業4

## **Max/Min Heap & Binary Search Tree**

**解題說明**



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

### 測試策略


---

## 結論



## 心得與問題


---

## 申論及開發報告


在資料儲存方面，程式使用線性容器來保存 heap 的內容，並利用索引的方式計算父節點與子節點的位置。這樣可以不需要真的建立樹的節點結構，就能完成 heap 的功能，也讓插入與刪除的操作比較容易實作。

在測試部分，程式分別建立 MinHeap 與 MaxHeap，輸入同樣的整數資料後，觀察輸出的分層結果是否符合 heap 的規則。之後再測試頂端元素的取得，以及刪除頂端元素後的結果是否仍然正確。從結果來看，程式可以正常完成題目要求的內容。

整體來說，這次開發重點在於 heap 的調整流程是否正確，尤其是插入後的往上調整與刪除後的往下調整。完成這次作業後，我對 heap 的基本原理與實作方式都有更進一步的認識。
