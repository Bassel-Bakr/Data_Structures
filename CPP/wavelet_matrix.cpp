#include "bits/stdc++.h"
using namespace std;
using ll = long long;

struct wavelet_matrix {
  int n;
  uint mask;
  uint logn;
  vector<int> z;
  vector<vector<int>> s;
  vector<vector<int>> b[2];

  template <class Iter>
  wavelet_matrix(Iter pl, Iter pr) {
    auto tmp = vector<int>(pl, pr);
    n = int(tmp.size());
    mask = ~0u;
    logn = 32;
    z.resize(logn);
    b[0].assign(logn + 1, vector<int>(n + 1));
    b[1].assign(logn + 1, vector<int>(n + 1));
    s.assign(logn + 1, vector<int>(n + 1));
    for (int j = logn; j >= 0; --j) {
      int cnt[2] = {};
      auto& zero = b[0][j];
      auto& one = b[1][j];
      auto f = [j](int x) { return (((x ^ (1u << 31)) >> (j - 1)) & 1) ^ 1; };
      for (int i = 0; i < n; ++i) {
        if (j) cnt[f(tmp[i]) ^ 1] += 1;
        zero[i + 1] = cnt[0];
        one[i + 1] = cnt[1];
        s[j][i + 1] = s[j][i] + tmp[i];
      }
      z[j] = cnt[0];
      if (j) stable_partition(tmp.begin(), tmp.end(), f);
    }
  }

  int sum(int l, int r, uint a, uint b) {
    return sum(logn, l, r + 1, a ^ (1u << 31), b ^ (1u << 31), 0, mask);
  }

  // [l, r)
  int sum(int i, int l, int r, uint a, uint t, uint A, uint Z) {
    if (l >= r) {
      return 0;
    }

    if (a <= A && Z <= t) {
      return s[i][r] - s[i][l];
    }

    uint mid = A + (Z - A) / 2;
    int x = 0;
    if (a <= mid) x += sum(i - 1, b[0][i][l], b[0][i][r], a, t, A, mid);
    if (mid < t)
      x += sum(i - 1, z[i] + b[1][i][l], z[i] + b[1][i][r], a, t, mid + 1, Z);
    return x;
  }

  int kth(int l, int r, int k) { return kth(logn, l, r + 1, k, 0, mask); }

  int kth(int i, int l, int r, int k, uint A, uint Z) {
    if (A == Z) return A ^ (1u << 31);
    uint mid = A + (Z - A) / 2;
    int zeros = b[0][i][r] - b[0][i][l];
    if (k <= zeros)
      return kth(i - 1, b[0][i][l], b[0][i][r], k, A, mid);
    else
      return kth(i - 1, z[i] + b[1][i][l], z[i] + b[1][i][r], k - zeros,
                 mid + 1, Z);
  }
};

int main() {
  cin.tie(0);
  cin.sync_with_stdio(0);

  int n = 8;
  auto a = vector<int>(n);
  iota(a.begin(), a.end(), -5);

  wavelet_matrix wm(a.begin(), a.end());

  cout << wm.sum(0, n - 1, -5, 0) << endl;

  for (int i = 0; i < n; ++i) {
    cout << wm.kth(0, n - 1, i + 1) << endl;
  }
}
