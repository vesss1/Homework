#include <iostream>
using namespace std;

int A(int x, int y) {
    int sx[10000], sy[10000]; // 模擬堆疊
    int top = 0;
    sx[top] = x;
    sy[top] = y;

    while (top >= 0) {
        x = sx[top];
        y = sy[top];
        top--;

        if (x == 0) {
            y = y + 1;
        } 
        else if (y == 0) {
            top++;
            sx[top] = x - 1;
            sy[top] = 1;
            continue;
        } 
        else {
            top++;
            sx[top] = x - 1;
            sy[top] = -1; // 標記等內層結果
            top++;
            sx[top] = x;
            sy[top] = y - 1;
            continue;
        }

        // 模擬回傳階段
        while (top >= 0 && sy[top] == -1) {
            x = sx[top];
            top--;
            top++;
            sx[top] = x;
            sy[top] = y;
            break;
        }
    }
    return y;
}

int main() {
    int x, y;
    cout << "Enter x and y: ";
    cin >> x >> y;
    cout << "Ackermann(" << x << "," << y << ") = " << A(x, y) << endl;
    return 0;
}
