#include <bits/stdc++.h>

using namespace std;

template<class T, class F = less<T>>
struct sparse_table {
  int n;
  F comp;
  vector<vector<T>> st;
  
  sparse_table() = default;
    
  template<class iter>
  sparse_table(iter l, iter r) {
    n = distance(l, r);
    int logn = 32 - __builtin_clz(n);
    st.assign(logn, vector<T>(n));
    
    // build
    auto it = l;
    for(int i = 0; i < n; ++i, ++it)
      st[0][i] = *it;
    
    for(int j = 1; (1<<j) <= n; ++j) {
      int b = 1 << j-1;
      int lim = 2*b-1;
      for(int i = 0; i + lim < n; ++i) {
        T const& x = st[j-1][i];
        T const& y = st[j-1][i+b];
        st[j][i] = comp(x, y) ? x : y;
      }
    }
  }
  
  T query(int l, int r) const {
    int b = 31 - __builtin_clz(r-l+1);
    T const& x = st[b][l];
    T const& y = st[b][r-(1<<b)+1];
    return comp(x, y) ? x : y;
  }
};

int main() {	
  vector<int> a = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  
  sparse_table<int> t(a.begin(), a.end());
  
  assert(t.query(0, 9) == 1);
}
