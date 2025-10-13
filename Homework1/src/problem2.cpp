#include <iostream>
#include <vector>
using namespace std;

void powerSet(vector<char> z, vector<char> cur, int x) {
    if (x == z.size()) {
        cout << "{";
        for (int i = 0; i < cur.size(); i++) {
            cout << cur[i];
            if (i < cur.size() - 1) cout << ",";
        }
        cout << "}" << endl;
        return;
    }
    // 不包含 z[x]
    powerSet(z, cur, x + 1);
    // 包含 z[x]
    cur.push_back(z[x]);
    powerSet(z, cur, x + 1);
}

int main() {
    vector<char> z = {'a', 'b', 'c'};
    vector<char> cur;
    powerSet(z, cur, 0);
    return 0;
}
