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
long long g[N][N];
bool vis[N];
Edge edge[E];

// biconnected 用
int dfn[N], low[N], timerCnt;
Edge stk[E];
int topStack;

// DFS
void dfs(int x) {
    vis[x] = true;
    cout << x << " ";

    for (int i = 0; i < n; i++) {
        if (g[x][i] != INF && x != i && !vis[i]) {
            dfs(i);
        }
    }
}

// BFS
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

// 連通分量
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

// DFS 產生樹
void spanningTree() {
    if (dir == 1) {
        cout << "Only for undirected graph\n";
        return;
    }

    int s;
    cout << "Start vertex: ";
    cin >> s;

    int q[N];
    int l = 0, r = 0;

    memset(vis, false, sizeof(vis));

    q[r++] = s;
    vis[s] = true;

    cout << "Spanning tree edges:\n";

    while (l < r) {
        int x = q[l++];

        for (int i = 0; i < n; i++) {
            if (g[x][i] != INF && x != i && !vis[i]) {
                vis[i] = true;
                q[r++] = i;
                cout << x << " - " << i << endl;
            }
        }
    }
}

// union find
int findp(int p[], int x) {
    if (p[x] == x) return x;
    p[x] = findp(p, p[x]);
    return p[x];
}

bool cmp(Edge a, Edge b) {
    return a.w < b.w;
}

// Kruskal
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

    cout << "Kruskal MST edges:\n";

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

// Prim
void prim() {
    if (dir == 1) {
        cout << "Only for undirected graph\n";
        return;
    }

    int s;
    cout << "Start vertex: ";
    cin >> s;

    long long d[N];
    int parent[N];
    bool used[N];

    for (int i = 0; i < n; i++) {
        d[i] = INF;
        parent[i] = -1;
        used[i] = false;
    }

    d[s] = 0;
    long long sum = 0;

    cout << "Prim MST edges:\n";

    for (int k = 0; k < n; k++) {
        int x = -1;

        for (int i = 0; i < n; i++) {
            if (!used[i] && (x == -1 || d[i] < d[x])) {
                x = i;
            }
        }

        if (x == -1 || d[x] == INF) break;

        used[x] = true;
        sum += d[x];

        if (parent[x] != -1) {
            cout << parent[x] << " - " << x << " : " << d[x] << endl;
        }

        for (int i = 0; i < n; i++) {
            if (!used[i] && g[x][i] != INF && g[x][i] < d[i]) {
                d[i] = g[x][i];
                parent[i] = x;
            }
        }
    }

    cout << "Total = " << sum << endl;
}

// Dijkstra
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

        for (int i = 0; i < n; i++) {
            if (!used[i] && (x == -1 || d[i] < d[x])) {
                x = i;
            }
        }

        if (x == -1 || d[x] == INF) break;

        used[x] = true;

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

// Bellman-Ford
void bellmanFord(int s) {
    long long d[N];

    for (int i = 0; i < n; i++) {
        d[i] = INF;
    }

    d[s] = 0;

    for (int k = 0; k < n - 1; k++) {
        for (int i = 0; i < m; i++) {
            int a = edge[i].u;
            int b = edge[i].v;
            long long w = edge[i].w;

            if (d[a] != INF && d[a] + w < d[b]) {
                d[b] = d[a] + w;
            }

            if (dir == 0) {
                if (d[b] != INF && d[b] + w < d[a]) {
                    d[a] = d[b] + w;
                }
            }
        }
    }

    bool neg = false;

    for (int i = 0; i < m; i++) {
        int a = edge[i].u;
        int b = edge[i].v;
        long long w = edge[i].w;

        if (d[a] != INF && d[a] + w < d[b]) {
            neg = true;
        }

        if (dir == 0) {
            if (d[b] != INF && d[b] + w < d[a]) {
                neg = true;
            }
        }
    }

    if (neg) {
        cout << "Negative cycle found\n";
        return;
    }

    cout << "Bellman-Ford:\n";

    for (int i = 0; i < n; i++) {
        cout << s << " to " << i << " = ";
        if (d[i] == INF) cout << "INF\n";
        else cout << d[i] << endl;
    }
}

// Floyd
void floyd() {
    long long d[N][N];

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            d[i][j] = g[i][j];
        }
    }

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

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (g[i][j] != INF) r[i][j] = 1;
            else r[i][j] = 0;
        }
    }

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

// AOV 拓撲排序
void aov() {
    if (dir == 0) {
        cout << "AOV needs directed graph\n";
        return;
    }

    int indeg[N];
    int q[N];
    int l = 0, r = 0;
    int ans[N];
    int cnt = 0;

    memset(indeg, 0, sizeof(indeg));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j && g[i][j] != INF) {
                indeg[j]++;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        if (indeg[i] == 0) {
            q[r++] = i;
        }
    }

    while (l < r) {
        int x = q[l++];
        ans[cnt++] = x;

        for (int i = 0; i < n; i++) {
            if (x != i && g[x][i] != INF) {
                indeg[i]--;

                if (indeg[i] == 0) {
                    q[r++] = i;
                }
            }
        }
    }

    if (cnt != n) {
        cout << "Graph has cycle\n";
        return;
    }

    cout << "Topological order:\n";

    for (int i = 0; i < cnt; i++) {
        cout << ans[i];
        if (i != cnt - 1) cout << " ";
    }

    cout << endl;
}

// AOE 關鍵路徑
void aoe() {
    if (dir == 0) {
        cout << "AOE needs directed graph\n";
        return;
    }

    int indeg[N];
    int q[N];
    int topo[N];
    int l = 0, r = 0, cnt = 0;

    memset(indeg, 0, sizeof(indeg));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j && g[i][j] != INF) {
                indeg[j]++;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        if (indeg[i] == 0) {
            q[r++] = i;
        }
    }

    while (l < r) {
        int x = q[l++];
        topo[cnt++] = x;

        for (int i = 0; i < n; i++) {
            if (x != i && g[x][i] != INF) {
                indeg[i]--;

                if (indeg[i] == 0) {
                    q[r++] = i;
                }
            }
        }
    }

    if (cnt != n) {
        cout << "Graph has cycle, cannot do AOE\n";
        return;
    }

    long long ve[N], vl[N];

    for (int i = 0; i < n; i++) {
        ve[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        int x = topo[i];

        for (int j = 0; j < n; j++) {
            if (x != j && g[x][j] != INF) {
                ve[j] = max(ve[j], ve[x] + g[x][j]);
            }
        }
    }

    long long projectTime = 0;

    for (int i = 0; i < n; i++) {
        projectTime = max(projectTime, ve[i]);
    }

    for (int i = 0; i < n; i++) {
        vl[i] = projectTime;
    }

    for (int i = n - 1; i >= 0; i--) {
        int x = topo[i];

        for (int j = 0; j < n; j++) {
            if (x != j && g[x][j] != INF) {
                vl[x] = min(vl[x], vl[j] - g[x][j]);
            }
        }
    }

    cout << "Project time = " << projectTime << endl;

    cout << "ve:\n";
    for (int i = 0; i < n; i++) {
        cout << i << ": " << ve[i] << endl;
    }

    cout << "vl:\n";
    for (int i = 0; i < n; i++) {
        cout << i << ": " << vl[i] << endl;
    }

    cout << "Critical edges:\n";

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j && g[i][j] != INF) {
                long long ee = ve[i];
                long long el = vl[j] - g[i][j];

                if (ee == el) {
                    cout << i << " -> " << j << " : " << g[i][j] << endl;
                }
            }
        }
    }
}

// Biconnected Components DFS
void bccDfs(int u, int parent) {
    dfn[u] = low[u] = ++timerCnt;

    for (int v = 0; v < n; v++) {
        if (g[u][v] == INF || u == v) continue;

        if (!dfn[v]) {
            stk[topStack].u = u;
            stk[topStack].v = v;
            stk[topStack].w = g[u][v];
            topStack++;

            bccDfs(v, u);

            low[u] = min(low[u], low[v]);

            if (low[v] >= dfn[u]) {
                cout << "Biconnected component: ";

                while (topStack > 0) {
                    Edge e = stk[--topStack];
                    cout << "(" << e.u << "," << e.v << ") ";

                    if (e.u == u && e.v == v) {
                        break;
                    }
                }

                cout << endl;
            }
        }
        else if (v != parent && dfn[v] < dfn[u]) {
            stk[topStack].u = u;
            stk[topStack].v = v;
            stk[topStack].w = g[u][v];
            topStack++;

            low[u] = min(low[u], dfn[v]);
        }
    }
}

// 雙連通分量
void biconnected() {
    if (dir == 1) {
        cout << "Only for undirected graph\n";
        return;
    }

    memset(dfn, 0, sizeof(dfn));
    memset(low, 0, sizeof(low));

    timerCnt = 0;
    topStack = 0;

    for (int i = 0; i < n; i++) {
        if (!dfn[i]) {
            bccDfs(i, -1);
        }
    }
}

// 印矩陣
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

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            g[i][j] = INF;
        }

        g[i][i] = 0;
    }

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
        cout << "9.Prim\n";
        cout << "10.Spanning Tree\n";
        cout << "11.Biconnected Components\n";
        cout << "12.Bellman-Ford\n";
        cout << "13.AOV\n";
        cout << "14.AOE\n";
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
        else if (op == 9) {
            prim();
        }
        else if (op == 10) {
            spanningTree();
        }
        else if (op == 11) {
            biconnected();
        }
        else if (op == 12) {
            int s;
            cin >> s;
            bellmanFord(s);
        }
        else if (op == 13) {
            aov();
        }
        else if (op == 14) {
            aoe();
        }

    } while (op != 0);

    return 0;
}
