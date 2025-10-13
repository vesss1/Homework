#41343127
---
##作業一 

##解題說明

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

---
心得與問題

這題剛開始看起來不難，但實際在跑的時候才發現 Ackermann 函數的遞迴層數非常深，一不小心就會超出系統限制。透過這次練習，我比較清楚地理解遞迴的執行順序，也學會用堆疊去模擬遞迴的過程。雖然程式不長，但要讓結果正確需要仔細追蹤變數的變化。整體來說，這題讓我更能體會到遞迴雖然簡潔，但效能和記憶體消耗都要特別注意。
