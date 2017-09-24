#include <bits/stdc++.h>
using namespace std;

template<int p, int mod>
class string_hash {
public:
  int n;
  vector<int> f;
  vector<int> h;

  constexpr static bool is_mersenne(ll x) {
    return not (x&(x+1));
  }

  constexpr static bool is_power_of_two(ll x) {
    return not (x&(x-1));
  }

  template<int z = mod, int o = __builtin_popcount(z)>
  inline static enable_if_t<is_mersenne(z), ll> fix(ll x) {
    x = (x >> o) + (x & mod);
    x = (x >> o) + (x & mod);
    return x;
  }

  template<int z = mod> 
  inline static enable_if_t<is_power_of_two(z), ll> fix(ll x) {
    return x & (mod-1);
  }
 
  template<int z = mod> 
  inline static enable_if_t<not (is_power_of_two(z) || is_mersenne(z)), ll> fix(ll x) {
    return x%mod;
  }

  template<class iter> 
  static int hash_of(iter l, iter r) {
    ll x = 0;
    for(iter it = l; it != r; ++it)
      x = fix(x*p + *it - 'a' + 1);
    return x;
  }

  template<class iter>
  string_hash(iter l, iter r) {
    n = distance(l, r);
    assert(n >= 0);
    h.assign(n+1, 0);
    f.assign(n+1, 1);

    ll x = 1, y = 0;
    iter it = l;
    for(int i = 0; i < n; ++i, ++it) {
      f[i+1] = x = fix(x * p);
      h[i+1] = y = fix(y*p + (*it) - 'a' + 1);
    }
  }

  int query(int l, int r) {
    ll x = fix(h[r+1] - fix(1ll * h[l] * f[r-l+1]));
    return fix(x+mod);
  }
};

int main() {
  string s = "hello";
  int x = string_hash<31,257>::hash_of(s.begin(), s.end());
  
  string_hash<31,257> hs(s.begin(), s.end());
  int y = hs.query(0, 4);
  
  assert(x == y);
}
