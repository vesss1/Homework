# 41343127
---
### 作業4

## **Max/Min Heap & Binary Search Tree**

**解題說明**

這題是用 heap 的方式來做最小堆積和最大堆積。

我先用 **vector** 來存 heap 裡面的資料，因為它可以像陣列一樣用索引存取，也能方便加入和刪除元素。

程式中用一個布林值來判斷目前要建立的是 MinHeap 還是 MaxHeap：

* 如果是 **true**，就表示 **MinHeap**
* 如果是 **false**，就表示 **MaxHeap**
