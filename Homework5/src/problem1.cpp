#include <algorithm>
#include <cstring>
#include <iostream>
using namespace std;

const int N = 100;
const int E = 1000;
const long long INF = 999999999;

struct Edge {
    int u, v;
    long long w;
};

int n, m, dir;
long long g[N][N];   // 鄰接矩陣
bool vis[N];         // 紀錄有沒有走過
Edge edge[E];        // 邊的資料

// 深度優先搜尋
void dfs(int x) {
    vis[x] = true;
    cout << x << " ";

    for (int i = 0; i < n; i++) {
        if (g[x][i] != INF && x != i && !vis[i]) {
            dfs(i);
        }
    }
}

// 廣度優先搜尋
void bfs(int s) {
    int q[N];
    int l = 0, r = 0;

    q[r++] = s;
    vis[s] = true;

    while (l < r) {
        int x = q[l++];
        cout << x << " ";

        for (int i = 0; i < n; i++) {
            if (g[x][i] != INF && x != i && !vis[i]) {
                vis[i] = true;
                q[r++] = i;
            }
        }
    }
}

// 找連通分量
void components() {
    if (dir == 1) {
        cout << "Only for undirected graph\n";
        return;
    }

    memset(vis, false, sizeof(vis));

    int cnt = 0;
    for (int i = 0; i < n; i++) {
        if (!vis[i]) {
            cnt++;
            cout << "Component " << cnt << ": ";
            dfs(i);
            cout << endl;
        }
    }
}

// 找集合代表
int findp(int p[], int x) {
    if (p[x] == x) return x;
    p[x] = findp(p, p[x]);
    return p[x];
}

// 依照權重排序
bool cmp(Edge a, Edge b) {
    return a.w < b.w;
}

// Kruskal 最小生成樹
void kruskal() {
    if (dir == 1) {
        cout << "Only for undirected graph\n";
        return;
    }

    Edge e2[E];
    for (int i = 0; i < m; i++) {
        e2[i] = edge[i];
    }

    sort(e2, e2 + m, cmp);

    int p[N];
    for (int i = 0; i < n; i++) {
        p[i] = i;
    }

    long long sum = 0;
    int cnt = 0;

    cout << "MST edges:\n";

    for (int i = 0; i < m; i++) {
        int a = findp(p, e2[i].u);
        int b = findp(p, e2[i].v);

        if (a != b) {
            p[a] = b;
            sum += e2[i].w;
            cnt++;

            cout << e2[i].u << " " << e2[i].v << " " << e2[i].w << endl;
        }
    }

    if (cnt != n - 1) {
        cout << "Graph is not connected\n";
    }

    cout << "Total = " << sum << endl;
}

// Dijkstra 單一起點最短路徑
void dijkstra(int s) {
    long long d[N];
    bool used[N];

    for (int i = 0; i < n; i++) {
        d[i] = INF;
        used[i] = false;
    }

    d[s] = 0;

    for (int k = 0; k < n; k++) {
        int x = -1;

        // 找目前距離最小的點
        for (int i = 0; i < n; i++) {
            if (!used[i] && (x == -1 || d[i] < d[x])) {
                x = i;
            }
        }

        if (x == -1 || d[x] == INF) break;

        used[x] = true;

        // 更新距離
        for (int i = 0; i < n; i++) {
            if (g[x][i] != INF && d[x] + g[x][i] < d[i]) {
                d[i] = d[x] + g[x][i];
            }
        }
    }

    for (int i = 0; i < n; i++) {
        cout << s << " to " << i << " = ";
        if (d[i] == INF) cout << "INF\n";
        else cout << d[i] << endl;
    }
}

// Floyd-Warshall 所有點對最短路徑
void floyd() {
    long long d[N][N];

    // 複製原本的矩陣
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            d[i][j] = g[i][j];
        }
    }

    // 嘗試經過中繼點 k
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (d[i][k] != INF && d[k][j] != INF) {
                    d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
                }
            }
        }
    }

    cout << "All shortest path:\n";

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (d[i][j] == INF) cout << "INF";
            else cout << d[i][j];

            if (j != n - 1) cout << " ";
        }
        cout << endl;
    }
}

// 可達矩陣
void reach() {
    int r[N][N];

    // 先記錄直接可達
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (g[i][j] != INF) r[i][j] = 1;
            else r[i][j] = 0;
        }
    }

    // 利用中繼點判斷是否可達
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (r[i][k] && r[k][j]) {
                    r[i][j] = 1;
                }
            }
        }
    }

    cout << "Reachability:\n";

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << r[i][j];
            if (j != n - 1) cout << " ";
        }
        cout << endl;
    }
}

// 印出鄰接矩陣
void matrix() {
    cout << "Matrix:\n";

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (g[i][j] == INF) cout << "INF";
            else cout << g[i][j];

            if (j != n - 1) cout << " ";
        }
        cout << endl;
    }
}

int main() {
    cin >> n >> m >> dir;

    // 初始化矩陣
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            g[i][j] = INF;
        }
        g[i][i] = 0;
    }

    // 輸入邊
    for (int i = 0; i < m; i++) {
        int a, b;
        long long w;

        cin >> a >> b >> w;

        g[a][b] = w;
        edge[i].u = a;
        edge[i].v = b;
        edge[i].w = w;

        if (dir == 0) {
            g[b][a] = w;
        }
    }

    int op;

    do {
        cout << "\n1.DFS\n";
        cout << "2.BFS\n";
        cout << "3.Components\n";
        cout << "4.Kruskal\n";
        cout << "5.Dijkstra\n";
        cout << "6.Floyd\n";
        cout << "7.Reach\n";
        cout << "8.Matrix\n";
        cout << "0.Exit\n";
        cout << "Choose: ";
        cin >> op;

        if (op == 1) {
            int s;
            cin >> s;
            memset(vis, false, sizeof(vis));
            dfs(s);
            cout << endl;
        }
        else if (op == 2) {
            int s;
            cin >> s;
            memset(vis, false, sizeof(vis));
            bfs(s);
            cout << endl;
        }
        else if (op == 3) {
            components();
        }
        else if (op == 4) {
            kruskal();
        }
        else if (op == 5) {
            int s;
            cin >> s;
            dijkstra(s);
        }
        else if (op == 6) {
            floyd();
        }
        else if (op == 7) {
            reach();
        }
        else if (op == 8) {
            matrix();
        }

    } while (op != 0);

    return 0;
}
