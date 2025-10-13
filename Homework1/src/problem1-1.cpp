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

