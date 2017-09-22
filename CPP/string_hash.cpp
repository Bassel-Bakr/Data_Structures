#include <bits/stdc++.h>
using namespace std;

template<int p=31, int mod=1000*1000*1000+7>
struct string_hash {
  using ll = long long;

  int n;
  vector<int> f;
  vector<int> h;

  template<class iter> 
  static int hash_of(iter l, iter r) {
    ll x = 0;
    for(iter it = l; it != r; ++it)
      x = (x*p + *it - 'a' + 1) % mod;
    return x;
  }

  template<class iter>
  string_hash(iter l, iter r) {
    n = distance(l, r);
    h.assign(n+1, 0);
    f.assign(n+1, 1);

    ll x = 1, y = 0;
    iter it = l;
    for(int i = 0; i < n; ++i) {
      x = x * p % mod;
      y = (y*p + *it++ - 'a' + 1) % mod;
      f[i+1] = x;
      h[i+1] = y;
    }
  }

  int query(int l, int r) {
    ll x = h[r+1];
    x -= 1ll * h[l] * f[r-l+1] % mod;
    x += mod;
    x %= mod;
    return x;
  }
};

int main() {
  string s = "hello";
  int x = string_hash<31,257>::hash_of(s.begin(), s.end());
  
  string_hash<31,257> hs(s.begin(), s.end());
  int y = hs.query(0, 4);
  
  assert(x == y);
}
