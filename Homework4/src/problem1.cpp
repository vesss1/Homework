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
