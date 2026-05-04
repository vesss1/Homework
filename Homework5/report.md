# 41343127 
# 41343150
---
# 作業5  **Graph**
## **解題說明**
這題主要是在做一個圖（Graph）的整合程式，把課堂上學到的幾個常見演算法全部寫在同一支程式裡面。  

一開始先讓使用者輸入節點數、邊數，還有圖是有向還是無向，接著把每一條邊存進去。  

這裡我是用鄰接矩陣來存圖，因為實作起來比較簡單，也比較方便後面直接套用各種演算法。  

整個程式是用選單的方式運作，使用者可以選擇要執行哪一種功能，例如 DFS、BFS、最小生成樹或最短路徑等等。  

### 功能部分，包含：  

- 用 DFS 和 BFS 來走訪圖  
- 找出無向圖的連通分量  
- 建立生成樹（Spanning Tree）  
- 用 Kruskal 和 Prim 找最小生成樹  
- 用 Dijkstra、Bellman-Ford 和 Floyd 找最短路徑  
- 判斷節點之間是否可以到達（Reachability）  
- 對有向圖做拓撲排序（AOV）  
- 找出關鍵路徑（AOE）  
- 分析雙連通分量（Biconnected Components）  

## **解題策略**
**這題我是用鄰接矩陣來存圖，因為寫法比較直觀，也比較方便拿來做 Floyd、Dijkstra、Reach 這些演算法。** 

一開始先輸入節點數、邊數，還有判斷是不是有向圖。  

如果是無向圖，就把 g[a][b] 和 g[b][a] 都存起來；如果是有向圖，就只存 g[a][b]。  

### 程式主要分成幾個部分：  

**1.** 先把圖存在 g[ ][ ] 裡面，沒有邊的地方用 INF 表示。  
**2.** DFS 和 BFS 用來走訪圖，並用 vis[ ] 紀錄哪些點已經走過。  
**3.** Connected Components 是重複使用 DFS，找出無向圖裡所有連在一起的區塊。  
**4.** Kruskal 和 Prim 用來找最小生成樹，主要是看哪些邊可以用最小成本把所有點連起來。  
**5.** Dijkstra、Bellman-Ford 和 Floyd 是用來找最短路徑。  
**6.** Reach 是判斷任兩點之間能不能到達。  
**7.** AOV 是用入度來做拓撲排序，判斷有向圖的執行順序。  
**8.** AOE 則是用拓撲排序算出最早時間、最晚時間，最後找出關鍵路徑。  
**9.** Biconnected Components 是用 DFS 搭配 dfn 和 low 來找雙連通分量。  

## 程式設計  
```cpp
//41343127
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
```

---
## 效能分析

| 功能 | 時間複雜度 | 空間複雜度 | 說明 |
|---|---:|---:|---|
| DFS | O(V²) | O(V) | 使用鄰接矩陣，每次找相鄰點都要掃一整列 |
| BFS | O(V²) | O(V) | 使用陣列當 queue，主要時間花在掃描矩陣 |
| Connected Components | O(V²) | O(V) | 對沒走過的點做 DFS |
| Spanning Tree | O(V²) | O(V) | 用 BFS 找出生成樹邊 |
| Kruskal | O(E log E) | O(E + V) | 先排序所有邊，再用 Union-Find 判斷是否成環 |
| Prim | O(V²) | O(V) | 每次用線性掃描找目前權重最小的點 |
| Dijkstra | O(V²) | O(V) | 沒有使用 priority queue，所以每次用掃描找最短距離 |
| Bellman-Ford | O(VE) | O(V) | 重複對所有邊做鬆弛 |
| Floyd-Warshall | O(V³) | O(V²) | 使用三層迴圈計算所有點對最短路徑 |
| Transitive Closure | O(V³) | O(V²) | 和 Floyd 類似，用三層迴圈判斷可達性 |
| AOV | O(V²) | O(V) | 使用鄰接矩陣計算入度與拓撲排序 |
| AOE | O(V²) | O(V) | 先做拓撲排序，再計算 ve 和 vl |
| Biconnected Components | O(V²) | O(V + E) | DFS 過程會掃描鄰接矩陣，並用 stack 存邊 |
| Adjacency Matrix | O(1) | O(V²) | 查詢兩點是否有邊很快，但空間較大 |

---
## 測試與驗證

| 測試項目 | 輸入內容 | 預期輸出 |
|----------|----------|----------|
| Graph 建立 | 5 6 0<br>0 1 2<br>0 2 3<br>1 2 1<br>1 3 4<br>2 4 5<br>3 4 2 | Matrix:<br>0 2 3 INF INF<br>2 0 1 4 INF<br>3 1 0 INF 5<br>INF 4 INF 0 2<br>INF INF 5 2 0 |
| DFS | 1 → 0 | 0 1 2 4 3 |
| BFS | 2 → 0 | 0 1 2 3 4 |
| Kruskal | 4 | 1-2 (1)<br>0-1 (2)<br>3-4 (2)<br>1-3 (4)<br>Total = 9 |
| Dijkstra | 5 → 0 | 0→0 = 0<br>0→1 = 2<br>0→2 = 3<br>0→3 = 6<br>0→4 = 8 |
| AOV | 13 | 0 1 2 3 4 5 |
| AOE | 14 | Project time = 10<br>Critical: 0 → 2 → 3 → 4 → 5 |

---
## 申論及開發報告

這次在實作 Graph 的過程中，我是以 **鄰接矩陣（Adjacency Matrix）** 作為主要的資料結構。雖然在某些情況下鄰接串列會比較省空間，但考量到本題需要實作像 Floyd、Reach 這類需要頻繁查詢兩點關係的演算法，使用矩陣在實作上會比較直覺，也比較不容易出錯。

在輸入資料時，程式會先讀入節點數、邊數以及圖的種類（有向或無向）。如果是無向圖，會同時設定 `g[a][b]` 和 `g[b][a]`；如果是有向圖，則只設定單方向。這樣在後續各個演算法中，就不需要再額外判斷邊的方向，邏輯會比較單純。

除了鄰接矩陣之外，我另外維護了一份 `Edge` 陣列來存所有邊。這樣做主要是為了方便像 Kruskal 和 Bellman-Ford 這類需要「逐邊處理」的演算法。如果只用矩陣，在這些地方反而會變得比較麻煩，因此這裡採用兩種資料結構搭配的方式。

在圖的走訪部分，我使用 DFS 和 BFS 兩種方式來實作。DFS 採用遞迴寫法，程式比較簡潔，而 BFS 則是用陣列模擬 queue。兩者都透過 `vis[]` 陣列來避免重複拜訪節點，確保走訪過程正確。

連通分量（Connected Components）則是重複呼叫 DFS，找出每一個尚未拜訪的節點所屬的區塊。這部分只在無向圖下執行，因為有向圖的情況會比較複雜。

在最小生成樹的部分，我實作了 Kruskal 和 Prim 兩種方法。Kruskal 是先將所有邊排序，再搭配 Union-Find 來避免產生環；Prim 則是從指定起點出發，每次選擇當前最小的邊來擴展生成樹。兩種方法雖然概念不同，但最終目標都是找到總權重最小的生成樹。

最短路徑方面，我分別實作了 Dijkstra、Bellman-Ford 和 Floyd。Dijkstra 適用於沒有負權邊的情況，實作上透過每次選擇目前距離最小的節點來更新其他點的距離；Bellman-Ford 則是透過多次鬆弛邊來處理負權邊，並額外檢查是否存在負環；Floyd 則是用三層迴圈的方式，計算所有點對之間的最短路徑。

另外在可達性（Reachability）部分，則是將矩陣轉為 0/1 表示是否有邊，再透過類似 Floyd 的方式來判斷任兩點之間是否可達。

在有向圖的應用中，我實作了 AOV（拓撲排序）與 AOE（關鍵路徑）。AOV 是利用入度（indegree）來決定節點的處理順序，如果最後無法排出完整序列，代表圖中存在環；AOE 則是在拓撲排序的基礎上，進一步計算每個節點的最早時間與最晚時間，最後找出關鍵邊。

最後，雙連通分量（Biconnected Components）是使用 DFS 搭配 `dfn` 和 `low` 的方式來實作，並利用 stack 紀錄邊的過程，在滿足條件時輸出一個雙連通分量。

在實作過程中，我有特別將權重型別設為 `long long`，並使用 `INF` 來表示不可到達，避免在做加總或比較時出現溢位或錯誤判斷的情況。

整體設計上，我將「圖的儲存」與「各種演算法」分開處理，再透過選單讓使用者選擇要執行的功能。這樣不但方便測試，也讓整個程式結構比較清楚，未來如果要再新增其他圖論演算法，也比較容易擴充。
