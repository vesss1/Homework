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
這題我是用鄰接矩陣來存圖，因為寫法比較直觀，也比較方便拿來做 Floyd、Dijkstra、Reach 這些演算法。  

一開始先輸入節點數、邊數，還有判斷是不是有向圖。  

如果是無向圖，就把 g[a][b] 和 g[b][a] 都存起來；如果是有向圖，就只存 g[a][b]。  

### 程式主要分成幾個部分：  

*1.*先把圖存在 g[ ][ ] 裡面，沒有邊的地方用 INF 表示。  
*2.*DFS 和 BFS 用來走訪圖，並用 vis[ ] 紀錄哪些點已經走過。  
*3.*Connected Components 是重複使用 DFS，找出無向圖裡所有連在一起的區塊。  
*4.*Kruskal 和 Prim 用來找最小生成樹，主要是看哪些邊可以用最小成本把所有點連起來。  
*5.*Dijkstra、Bellman-Ford 和 Floyd 是用來找最短路徑。  
*6.*Reach 是判斷任兩點之間能不能到達。  
*7.*AOV 是用入度來做拓撲排序，判斷有向圖的執行順序。  
*8.*AOE 則是用拓撲排序算出最早時間、最晚時間，最後找出關鍵路徑。  
*9.*Biconnected Components 是用 DFS 搭配 dfn 和 low 來找雙連通分量。  

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

**時間複雜度：** `O(1)`
---

### `Push()`
插入資料時，先把新元素放到陣列最後面，然後往上調整到正確位置。

在最差情況下，新元素可能一路從最底層換到最上層。  
heap 的高度大約是 `log n`，所以最多調整 `log n` 次。

**時間複雜度：** `O(log n)`

---

### `Pop()`
刪除頂端元素時，先把最後一個元素補到根節點，再往下調整。

在最差情況下，這個元素可能一路往下移到最底層，最多也是調整 `log n` 次。

**時間複雜度：** `O(log n)`

---

## 整體分析
heap 的優點是可以快速取得最小值或最大值：

- 取得頂端元素很快：`O(1)`
- 插入與刪除也不會太慢：`O(log n)`

所以 heap 很適合拿來做優先佇列這類需要常常取出最大值或最小值的情況。

---
---

### `Top()`
直接回傳陣列最前面的元素，也就是 heap 的根節點。

- MinHeap 回傳最小值
- MaxHeap 回傳最大值

不需要比較其他元素。

**時間複雜度：** `O(1)`

### 測試與驗證
這題的測試方式，主要是確認 MinHeap 和 MaxHeap 是否都能正常運作。

測試時先輸入一組整數資料，並把同一組資料同時放進 MinHeap 和 MaxHeap。

| 測試案例 | 組數 | 測資 | MinHeap Tree | MaxHeap Tree |
|---|---:|---|---|---|
| 測試一 | 5 | 9 3 7 1 6 | Level 0: 1<br>Level 1: 3 7<br>Level 2: 9 6 | Level 0: 9<br>Level 1: 6 7<br>Level 2: 1 3 |
| 測試二 | 7 | -1 44 -5 100 51 -101 8 | Level 0: -101<br>Level 1: 44 -5<br>Level 2: 100 51 -1<br>Level 3: 8 | Level 0: 100<br>Level 1: 51 8<br>Level 2: -1 44 -101<br>Level 3: -5 |

---

## 結論

這題成功完成了 MinHeap 和 MaxHeap 的基本功能，包括插入、取得頂端元素、判斷是否為空，以及刪除頂端元素。

從測試結果可以看出，程式能正確維持 heap 的規則。
MinHeap 可以正確保留最小值在最上面，MaxHeap 可以正確保留最大值在最上面，而且刪除頂端元素後也能正常重新調整。

整體來說，這份程式已經完成題目要求的功能，也讓我更了解 heap 的運作方式。

---

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#  Binary Search Tree

## 解題說明

第 2 題分成兩個部分。

### (a)

這一小題是要觀察 Binary Search Tree 在隨機插入資料時，樹高和 `log2(n)` 的關係。

程式先建立一棵空的 BST，然後對不同的 `n` 值進行隨機插入。  
每次插入完成後，計算該樹的高度 `height`，再將樹高除以 `log2(n)`，最後把不同 `n` 的結果列出，方便後續整理成圖表。  
這題的重點不是樹的外觀，而是要觀察這個比值是否大致接近某個固定值。

### (b)

這一小題是要實作 BST 的刪除功能，也就是刪除鍵值為 `k` 的節點。

刪除節點時，會遇到三種情況：

1. 該節點沒有子節點，直接刪除
2. 該節點只有一個子節點，用子節點取代它
3. 該節點有兩個子節點，找右子樹中最小的節點來取代，再刪掉那個替代節點

這樣做可以在刪除節點之後，仍然維持 BST 原本的性質。

---

### 程式設計 (a) 
```cpp
#include <cmath>
#include <iostream>
#include <random>
using namespace std;

class Node {
public:
    int data;
    Node* left;
    Node* right;

    Node(int x) {
        data = x;
        left = NULL;
        right = NULL;
    }
};

class BST {
private:
    Node* root;

    Node* insert(Node* node, int x) {
        if (node == NULL) {
            return new Node(x);
        }

        if (x < node->data) {
            node->left = insert(node->left, x);
        }
        else {
            node->right = insert(node->right, x);
        }

        return node;
    }

    int height(Node* node) {
        if (node == NULL) {
            return 0;
        }

        int lh = height(node->left);
        int rh = height(node->right);

        if (lh > rh) {
            return lh + 1;
        }
        else {
            return rh + 1;
        }
    }

    void clear(Node* node) {
        if (node == NULL) {
            return;
        }

        clear(node->left);
        clear(node->right);
        delete node;
    }

public:
    BST() {
        root = NULL;
    }

    ~BST() {
        clear(root);
    }

    void insert(int x) {
        root = insert(root, x);
    }

    int getHeight() {
        return height(root);
    }
};

int main() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 1000000);

    cout << "n\theight\theight/log2(n)" << endl;

    for (int n = 100; n <= 10000; ) {
        BST tree;

        for (int i = 0; i < n; i++) {
            int x = dist(gen);
            tree.insert(x);
        }

        int h = tree.getHeight();
        double ratio = h / log2((double)n);

        cout << n << '\t' << h << '\t' << ratio << endl;

        if (n == 100) {
            n = 500;
        }
        else {
            n += 500;
        }
    }

    return 0;
}
```
---

### 程式設計 (b) 
```cpp
#include <iostream>
#include <string>
using namespace std;

class Node {
public:
    int data;
    Node* left;
    Node* right;

    Node(int x) {
        data = x;
        left = NULL;
        right = NULL;
    }
};

class BST {
private:
    Node* root;

    Node* insert(Node* node, int x) {
        if (node == NULL) {
            return new Node(x);
        }

        if (x < node->data) {
            node->left = insert(node->left, x);
        }
        else if (x > node->data) {
            node->right = insert(node->right, x);
        }

        return node;
    }

    Node* findMin(Node* node) {
        while (node->left != NULL) {
            node = node->left;
        }
        return node;
    }

    Node* remove(Node* node, int k) {
        if (node == NULL) {
            return NULL;
        }

        if (k < node->data) {
            node->left = remove(node->left, k);
        }
        else if (k > node->data) {
            node->right = remove(node->right, k);
        }
        else {
            if (node->left == NULL && node->right == NULL) {
                delete node;
                return NULL;
            }
            else if (node->left == NULL) {
                Node* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == NULL) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            else {
                Node* temp = findMin(node->right);
                node->data = temp->data;
                node->right = remove(node->right, temp->data);
            }
        }

        return node;
    }

    void inorder(Node* node) {
        if (node == NULL) {
            return;
        }

        inorder(node->left);
        cout << node->data << " ";
        inorder(node->right);
    }

    void clear(Node* node) {
        if (node == NULL) {
            return;
        }

        clear(node->left);
        clear(node->right);
        delete node;
    }

public:
    BST() {
        root = NULL;
    }

    ~BST() {
        clear(root);
    }

    void insert(int x) {
        root = insert(root, x);
    }

    void remove(int k) {
        root = remove(root, k);
    }

    void print() {
        inorder(root);
        cout << endl;
    }
};

int main() {
    BST tree;

    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);

    cout << "Before delete: ";
    tree.print();

    tree.remove(50);

    cout << "After delete: ";
    tree.print();

    return 0;
}
```

---
### 效能分析

# 第 2 題 (a)：Binary Search Tree 

## 插入 `insert()`
每插入一個節點，都要從根節點開始往下找位置。

它的時間和樹高有關，所以是：

**時間複雜度：** `O(h)`

其中 `h` 是樹高。

---

## 計算高度 `height()`
這個函式需要走訪整棵樹的每個節點，才能算出最大高度。

如果樹中有 `n` 個節點，就要全部看過一次。

**時間複雜度：** `O(n)`

---

## 整個實驗的成本
這題會對不同的 `n` 做隨機插入。

假設有 `n` 個節點：

- 插入全部資料大約要做 `n` 次插入
- 每次插入平均大約是 `O(log n)`
- 所以平均建立整棵樹大約是 `O(n log n)`

之後再算一次高度：

- `O(n)`

所以單次測試整體大約是：

**平均時間複雜度：** `O(n log n)`

---

# 第 2 題 (b)：Binary Search Tree 

## 刪除 `remove(k)`
刪除節點時，第一步要先找到鍵值為 `k` 的節點。  
找到後再依照節點情況做處理：

- 沒有子節點：直接刪除
- 只有一個子節點：用子節點補上
- 有兩個子節點：找右子樹最小值來取代，再刪除那個替代節點

這整個過程主要還是和樹高有關。

**時間複雜度：** `O(h)`

其中 `h` 是樹高。

---

## 平均情況
如果樹接近平衡，樹高大約是：

`h = O(log n)`

所以刪除的平均效率是：

**平均時間複雜度：** `O(log n)`

---

## 最差情況
如果樹退化成像鏈結串列一樣，樹高可能變成：

`h = O(n)`

所以最差情況下：

**最差時間複雜度：** `O(n)`

---

## 整體分析
BST 的刪除效率其實不只和資料個數有關，更重要的是樹高。

也就是說：

- 樹越平衡，刪除越快
- 樹越歪，刪除越慢

所以 BST 雖然操作方式清楚，但它不保證一定維持高效率，這也是它和平衡樹最大的差別之一。

---

## 測試與驗證

### (a)

這一部分主要是驗證程式在不同節點數量下，是否能正確建立 BST，並算出對應的樹高與 `height / log2(n)`。

測試時依照題目要求，分別對不同的 `n` 值進行隨機插入，例如：

- `100`
- `500`
- `1000`
- `1500`
- `2000`
- ...
- `10000`

每次插入完成後，記錄以下資料：

- 節點數 `n`
- BST 的高度 `height`
- `height / log2(n)` 的值

再將各組結果整理成表格或圖表，觀察這個比值是否大致維持在接近的範圍內。  
如果結果沒有隨著 `n` 增加而出現過大的變動，就表示 BST 的高度和 `log2(n)` 的成長趨勢是接近的，符合題目的觀察方向。

另外，由於這一題使用的是隨機資料，因此每次執行的結果可能不會完全相同，但整體趨勢應該相近。

### (b)

這一部分主要是驗證 BST 刪除函式是否能在不同情況下正常運作，並且在刪除後仍然維持 BST 的規則。

測試時先建立一棵 BST，接著分別測試以下三種刪除情況：

- 刪除沒有子節點的節點
- 刪除只有一個子節點的節點
- 刪除有兩個子節點的節點

例如可先插入以下資料：

```text
50 30 70 20 40 60 80
接著進行不同測試：

* 刪除 20：測試刪除葉節點
* 刪除 30：測試刪除只有一個子節點的節點
* 刪除 50：測試刪除有兩個子節點的節點

每次刪除後，都檢查樹中的資料排列是否仍然符合 BST 的性質，也就是左子樹的值小於根節點，右子樹的值大於根節點。
如果刪除後的結果仍然正確，表示刪除函式在各種情況下都能正常運作。

透過這樣的方式，可以驗證第 (b) 題的刪除功能是否完整且正確。
```
---

## 結論

第 2 題主要是在練習 Binary Search Tree 的建立、高度觀察與刪除操作。

在 (a) 中，透過不同節點數量的隨機插入，可以看出 BST 的樹高會隨著資料量增加而變高，但整體來看，`height / log2(n)` 的比值通常會維持在接近的範圍內。這表示在隨機情況下，BST 的高度成長和 `log n` 之間有明顯關係。

在 (b) 中，透過刪除不同類型的節點，可以驗證刪除函式是否能正確處理各種情況。無論是刪除葉節點、只有一個子節點的節點，還是有兩個子節點的節點，只要處理方式正確，刪除後仍然可以維持 BST 的性質。

整體來說，這題讓我更了解 BST 的基本特性，也更清楚它在實作時需要注意的地方，特別是樹高變化和刪除節點的處理方式。

---

## 心得與問題

### **MaxHeap & MaxHeap**

這次第 1 題主要是在實作 MaxHeap 和 MinHeap。雖然一開始看起來只是做插入和刪除，但實際寫起來還是需要很小心，因為 heap 並不是單純把資料放進去就好，而是每次操作後都要重新維持它原本的規則。

在實作過程中，我覺得最重要的部分是插入後的往上調整和刪除後的往下調整。插入新資料時，要一路和父節點比較；如果不符合規則，就要交換位置。刪除頂端元素時，也不能直接把資料拿掉就結束，還要把最後一個元素補上來，再慢慢往下調整。這兩個步驟雖然概念不算太難，但如果索引位置算錯，整個結果就會不正確。

另外，我也更清楚看到 MinHeap 和 MaxHeap 的差別其實主要在比較方向。MinHeap 是讓較小的值往上移，MaxHeap 則是讓較大的值往上移。雖然程式架構很像，但如果比較條件沒有分清楚，就很容易出現結果相反的情況。

這題比較容易遇到的問題，是在調整節點時左右子節點的判斷。因為在往下調整時，不只是要看有沒有子節點，還要決定應該和左邊還是右邊交換，這部分如果寫得不夠仔細，就可能造成 heap 的規則被破壞。

整體來說，這題讓我更了解 heap 的運作方式，也讓我發現資料結構不只是背定義而已，真正實作時要把每個步驟都想清楚。做完之後，我對 heap 在插入、刪除和維持結構這幾個部分都有更明確的理解。

### **Binary Search Tree**

這次第 2 題讓我對 Binary Search Tree 有更完整的理解，不只是知道它的定義而已，也比較能看懂它在程式裡是怎麼運作的。和前面的 heap 比起來，BST 更需要去注意左右子樹的關係，因為每插入一個資料，都要判斷應該往左邊還是右邊放，這樣最後才能維持二元搜尋樹原本的性質。

在 (a) 的部分，原本我以為只要把資料一直插進去就好，但實際做完之後才發現，這題真正要看的不是插入本身，而是樹高的變化。因為題目不是單純要建立 BST，而是要透過不同的 `n` 去觀察 `height / log2(n)` 的結果。做完之後我比較能理解，為什麼老師會要我們看這個比值，因為這樣可以更清楚看出樹高和資料量之間的關係，也能知道 BST 在隨機情況下大概會有什麼表現。

在 (b) 的部分，我覺得最麻煩的是刪除節點。插入其實還算直接，但刪除會分成很多情況，像是沒有子節點、只有一個子節點，還有兩個子節點，每一種處理方式都不一樣。尤其是有兩個子節點的時候，不能直接刪掉，而是要找一個合適的節點來補原本的位置，這一段在想邏輯的時候比較容易亂掉。

這題在實作時比較容易遇到的問題，就是遞迴流程和樹的結構變化。如果只是在紙上看，會覺得做法還算清楚，但真的寫成程式之後，常常要很小心回傳的節點位置對不對，不然刪完之後整棵樹可能就會接錯。還有一點是，雖然 BST 平均情況下效率不錯，但它其實不保證平衡，所以資料插入方式不同，結果也可能差很多，這也是這題讓我印象比較深的地方。

整體來說，這題讓我更清楚 BST 的特性，也讓我知道理論和真正寫程式之間還是有差別。尤其是在刪除操作這部分，如果沒有真的自己做一次，可能不會發現裡面有這麼多細節要注意。

---

## 申論及開發報告

### **MaxHeap & MaxHeap**

第 1 題的主要目標是完成 MaxHeap 與 MinHeap 的基本功能。題目要求的重點在於實作 heap 的插入、刪除、判斷是否為空，以及取得頂端元素等操作，並讓程式能正確維持 heap 的結構。

在程式設計上，heap 使用陣列的方式來存放資料，因為這樣可以直接利用索引位置計算父節點與左右子節點的位置，不需要另外建立複雜的樹節點結構。這種做法也是 heap 常見的實作方式，可以讓插入與刪除的操作比較容易完成。

在插入功能方面，程式先將新元素放到目前資料的最後面，再透過往上調整的方式，讓新元素移動到適合的位置。如果是 MinHeap，就讓較小的值往上移；如果是 MaxHeap，就讓較大的值往上移。這樣可以確保頂端元素始終是目前最小值或最大值。

在刪除功能方面，程式先移除頂端元素，並將最後一個元素補到根節點，再透過往下調整來恢復 heap 的規則。在調整過程中，必須比較左右子節點，找出更適合交換的節點位置，才能讓整體結構保持正確。

測試時使用同一組資料分別建立 MinHeap 和 MaxHeap，接著觀察 `IsEmpty()`、`Top()` 與 `Pop()` 的執行結果是否正確。從測試結果可以確認，MinHeap 能正確取得最小值，MaxHeap 能正確取得最大值，刪除頂端元素後也能維持正確的 heap 性質。

透過這次開發，可以更清楚了解 heap 的基本原理與實作方式，也能看出它在處理最大值或最小值查詢時的便利性。這題不只是完成程式功能，也讓我更熟悉 heap 在資料結構中的應用方式。

### **Binary Search Tree**

第 2 題的內容主要和 Binary Search Tree 有關，分成兩個部分，第一部分是觀察樹高的變化，第二部分是完成刪除節點的功能。

在 (a) 的開發過程中，程式先建立一棵空的 BST，之後再利用隨機方式產生資料，並依序插入樹中。每當完成一組指定大小的測試後，就去計算整棵樹的高度，再進一步算出 `height / log2(n)` 的值。這一部分的目的，不是為了得到某一棵固定形狀的樹，而是想觀察在隨機插入下，BST 的高度大致會如何成長。透過多組不同 `n` 的測試，可以把結果整理成表格，之後再拿來做圖或分析整體趨勢。

在 (b) 的部分，主要是設計 BST 的刪除函式。刪除時要先找到鍵值為 `k` 的節點，接著依照該節點的情況做不同處理。如果節點沒有子節點，就直接刪除；如果只有一個子節點，就讓那個子節點接到原本的位置；如果有兩個子節點，則先找右子樹中最小的節點，將其值放到目前節點，再到右子樹中刪除原本那個替代節點。這樣做的目的是在刪除後仍能保持 BST 左小右大的性質。

在測試驗證部分，(a) 著重於結果的統計與觀察，確認程式是否能正確輸出不同 `n` 下的樹高與比值；(b) 則著重於刪除結果是否正確，會分別測試不同類型的節點刪除情況，確認刪除後的樹仍然符合 BST 的規則。這兩部分雖然方向不同，但都能幫助理解 BST 的核心特性。

透過這次開發，可以更清楚看出 BST 的優點與限制。它的結構很適合做搜尋、插入與刪除，但整體效率仍然會受到樹高影響。如果資料分布平均，操作效率會比較好；但若樹變得不平衡，效率就可能下降。因此，這題除了是在練習 BST 的基本操作外，也讓人更了解它在實際使用時需要注意的地方。
