**41343127**
---
作業一 

**解題說明**

本程式利用遞迴方式實作 Ackermann 函數：  
- 若 x = 0，回傳 y + 1。  
- 若 y = 0，回傳 A(x−1, 1)。  
- 否則回傳 A(x−1, A(x, y−1))。 

Ackermann 函數是經典的遞迴範例，能展現遞迴的深度與函數呼叫堆疊概念。  

---

### 程式設計  
```cpp
#include <iostream>
using namespace std;

int A(int x, int y) {
    if (x == 0)
        return y + 1;
    else if (y == 0)
        return A(x - 1, 1);
    else
        return A(x - 1, A(x, y - 1));
}

int main() {
    int x, y;
    cout << "Enter x and y: ";
    cin >> x >> y;
    cout << "Ackermann(" << x << "," << y << ") = " << A(x, y) << endl;
    return 0;
}
```

| 輸入 (x, y) | 輸出結果 |
| --------- | ---- |
| 0, 0      | 1    |
| 1, 2      | 4    |
| 2, 2      | 7    |

---
結論

本程式可正確計算 Ackermann 函數結果，但當 x 或 y 較大時會導致遞迴過深，可能造成堆疊溢位。
