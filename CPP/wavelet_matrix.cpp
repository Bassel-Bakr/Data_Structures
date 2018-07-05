#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct wavelet_matrix {
  int n;
  int blk;
  int logn;
  vector<int> z;
  vector<vector<pair<int, int>>> b;

  wavelet_matrix(vector<int>& a) {
    n = a.size();
    blk = (n + 32) >> 5;
    logn = 32 - __builtin_clz(*max_element(a.begin(), a.end()));
    // resize
    z.assign(logn, 0);
    b.assign(logn, vector<pair<int, int>>(blk));
    // build
    for (int bit = logn - 1; bit >= 0; --bit) {
      auto f = [bit](int x) { return not(x >> bit & 1); };
      auto& b = this->b[bit];
      for (int i = 0; i < n; ++i)
        if (f(a[i])) b[i >> 5].first |= 1u << (i & 31);
      for (int i = 1; i < blk; ++i)
        b[i].second = b[i - 1].second + __builtin_popcount(b[i - 1].first);
      auto zeros = stable_partition(a.begin(), a.end(), f);
      z[bit] = distance(a.begin(), zeros);
    }
  }

  int rank0(int bit, int i) {
    return b[bit][i >> 5].second +
           __builtin_popcount(b[bit][i >> 5].first & ((1u << (i & 31)) - 1u));
  }

  int rank1(int bit, int i) { return i - rank0(bit, i); }

  // kth number in range [l, r]
  int kth(int l, int r, int k) {
    r++;  // switch to [l, r) indexing for convinence
    int ans = 0;
    for (int bit = logn - 1; bit >= 0; --bit) {
      int lf = rank0(bit, l);
      int rg = rank0(bit, r);
      if (k <= rg - lf) {
        l = lf;
        r = rg;
      } else {
        ans |= 1u << bit;
        k -= rg - lf;
        l += z[bit] - lf;
        r += z[bit] - rg;
      }
    }
    return ans;
  }
};

int main() {
  cin.tie(0);
  cin.sync_with_stdio(0);

  int n, m;
  cin >> n >> m;
  vector<int> a(n);
  for (auto& x : a) cin >> x;
  auto b = a;
  sort(b.begin(), b.end());
  b.resize(unique(b.begin(), b.end()) - b.begin());
  for (auto& x : a) x = lower_bound(b.begin(), b.end(), x) - b.begin();
  wavelet_matrix wx(a);
  while (m--) {
    int l, r, k;
    cin >> l >> r >> k;
    cout << b[wx.kth(l - 1, r - 1, k)] << '\n';
  }
}
