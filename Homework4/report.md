# 41343127
---
### 作業4

## **Max/Min Heap & Binary Search Tree**

**解題說明**

這題是用 heap 的方式來做最小堆積和最大堆積。

程式中用一個布林值來判斷目前要建立的是 MinHeap 還是 MaxHeap：

* 如果是 **true**，就表示 **MinHeap**
* 如果是 **false**，就表示 **MaxHeap**

在插入新資料時，先把資料放到最後面，再一路往上和父節點比較；如果不符合 heap 的規則，就交換位置，直到回到正確的位置為止。

在刪除頂端元素時，先把根節點和最後一個元素交換，再刪掉最後一個元素，接著從根節點開始往下調整，讓整個 heap 重新符合規則。

這份程式可以建立 MinHeap 和 MaxHeap，並且能顯示頂端元素，還可以在刪除頂端元素後，輸出調整過後的結果。

---

### 程式設計  
```cpp
#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;

template <class T>
class Heap {
private:
    vector<T> h;
    bool isMin;

    bool cmp(const T& a, const T& b) const {
        return isMin ? a < b : a > b;
    }

    void up(int i) {
        while (i > 0) {
            int p = (i - 1) / 2;
            if (cmp(h[i], h[p])) {
                swap(h[i], h[p]);
                i = p;
            }
            else {
                break;
            }
        }
    }

    void down(int i) {
        int n = h.size();
        while (2 * i + 1 < n) {
            int l = 2 * i + 1;
            int r = 2 * i + 2;
            int t = l;

            if (r < n && cmp(h[r], h[l])) {
                t = r;
            }

            if (cmp(h[t], h[i])) {
                swap(h[t], h[i]);
                i = t;
            }
            else {
                break;
            }
        }
    }

public:
    Heap(bool m = true) : isMin(m) {}

    bool Empty() const {
        return h.empty();
    }

    const T& Top() const {
        if (Empty()) {
            throw runtime_error("Heap is empty!");
        }
        return h[0];
    }

    void Push(const T& v) {
        h.push_back(v);
        up(h.size() - 1);
    }

    void Pop() {
        if (Empty()) {
            throw runtime_error("Heap is empty!");
        }
        swap(h[0], h.back());
        h.pop_back();
        if (!Empty()) {
            down(0);
        }
    }

    void in_p(int n) {
        T x;
        for (int i = 0; i < n; i++) {
            cin >> x;
            Push(x);
        }
    }

    void printTree() {
        int n = h.size();
        int lv = 0;
        int idx = 0;

        while (idx < n) {
            int cnt = 1 << lv;
            cout << "L" << lv << ": ";
            for (int i = 0; i < cnt && idx < n; i++) {
                cout << h[idx++] << " ";
            }
            cout << endl;
            lv++;
        }
    }
};

int main() {
    int n;

    cout << "請輸入測資組數: ";
    cin >> n;

    Heap<int> mn(true);
    cout << "請輸入" << n << "個測資數 (MinHeap):" << endl;
    mn.in_p(n);

    cout << endl << "MinHeap" << endl;
    mn.printTree();
    cout << endl << "最小元素: " << mn.Top() << endl;
    mn.Pop();
    cout << "刪除最小元素後的樹:" << endl;
    mn.printTree();

    Heap<int> mx(false);
    cout << endl << "請輸入" << n << "個測資數 (MaxHeap):" << endl;
    mx.in_p(n);

    cout << endl << "MaxHeap" << endl;
    mx.printTree();
    cout << endl << "最大元素: " << mx.Top() << endl;
    mx.Pop();
    cout << "刪除最大元素後的樹:" << endl;
    mx.printTree();

    return 0;
}
```
