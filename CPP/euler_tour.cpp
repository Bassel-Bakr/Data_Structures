#include <bits/stdc++.h>

using namespace std;
using ll = long long;

int const N = 100 * 1000 + 16;
int const M = 1000 * 1000 * 1000 + 7;

int n;
vector<int> g[N];
vector<int> dist, tin, tour;
vector<vector<int>> st;

int dfs(int u, int p) {
  dist[u] = dist[p] + 1;

  tin[u] = tour.size();
  tour.emplace_back(u);

  for(int v : g[u]) {
    if(v == u)
      continue;
    dfs(v, u);
    tour.emplace_back(u);
  }
}

void build() {
  int n = tour.size();
  int logn = 32 - __builtin_clz(n);

  st.assign(logn, vector<int>(n));

  copy(tour.begin(), tour.end(), st[0].begin());

  for(int j = 1; (1<<j) <= n; ++j) {
    int b = 1 << (j-1);
    int lim = b + b - 1;
    for(int i = 0; i + lim < n; ++i) {
      int x = st[j-1][i];
      int y = st[j-1][i+b];
      st[j][i] = dist[x] < dist[y] ? x : y;
    }
  }
}

int lca(int u, int v) {
  int l = tin[u];
  int r = tin[v];

  if(l > r)
    swap(l, r);

  int b = 31 - __builtin_clz(r-l+1);
  int x = st[b][l];
  int y = st[b][r-(1<<b)+1];
  return dist[x] < dist[y] ? x : y;
}

int distance(int u, int v) {
  return dist[u] + dist[v] - 2 * dist[lca(u, v)];
}

int main() {
  cin.tie(0);
  cin.sync_with_stdio(0);

  cin >> n;
  for(int i = 1; i < n; ++i) {
    int u, v;
    cin >> u >> v;
    g[u].emplace_back(v);
    g[v].emplace_back(u);
  }

  tin.resize(n+1);
  dist.resize(n+1);
  
  tour.reserve(2*n-1);

  dfs(1, 0);
  build();


}
