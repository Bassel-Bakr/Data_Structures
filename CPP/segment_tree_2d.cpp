#include <bits/stdc++.h>
using namespace std;

struct segment {
  int n;
  vector<int> t;

  segment(int s) {
    n = s;
    t.assign(n+n, 0);
  }

  void update(int l, int r, int v) {
    for(l += n, r += n; l <= r; l >>= 1, r >>= 1) {
      if(l&1) t[l++] ^= v;
      if((r&1)^1) t[r--] ^= v;
    }
  }

  int query(int i) {
    int sum = 0;    
    for(i += n; i > 0; i >>= 1)
      sum ^= t[i];
    return sum;
  }
};

struct segment2D {
  int n, m;
  vector<segment> t;

  segment2D(int x, int y) {
    n = x;
    m = y;
    t.assign(n+n, segment(m));
  }

  void add(int x1, int y1, int x2, int y2, int v) {
    for(x1 += n, x2 += n; x1 <= x2; x1 >>= 1, x2 >>= 1) {
      if(x1&1) t[x1++].update(y1, y2, v);
      if((x2&1)^1) t[x2--].update(y1, y2, v);
    }
  }

  void rem(int x1, int y1, int x2, int y2, int v) {
    add(x1, y1, x2, y2, v);
  }

  int query(int x, int y) {
    int sum = 0;
    for(x += n; x > 0; x >>= 1)
      sum ^= t[x].query(y);
    return sum;
  }
};

int n, m, q;

int main() {
  cin.tie(0);
  cin.sync_with_stdio(0);

  cin >> n >> m >> q;
  segment2D a(n, m), a_xor(n, m);

  random_device rd;
  mt19937 rand(rd());
  set<tuple<int,int,int,int,int,int>> s;

  int mn = 0;
  while(q--) {
    int t, x1, y1, x2, y2;
    cin >> t >> x1 >> y1 >> x2 >> y2;
    --x1;--y1;--x2;--y2;

    if(t == 1) {
      auto f = s.lower_bound(make_tuple(x1, y1, x2, y2, mn, mn));
      if(f != s.end()) {
        int a1, b1, a2, b2;
        tie(a1, b1, a2, b2, ignore, ignore) = *f;
        if(tie(a1, b1, a2, b2) == tie(x1, y1, x2, y2))
          continue;
      }


      int p1 = q+1000000, p2 = rand();
      mn = min(mn, p2);
      s.insert(make_tuple(x1, y1, x2, y2, p1, p2));

      a.add(x1, y1, x2, y2, p1);
      a_xor.add(x1, y1, x2, y2, p2);
    } else if(t == 2) {
      auto f = s.lower_bound(make_tuple(x1, y1, x2, y2, mn, mn));
      int p1, p2;
      tie(ignore, ignore, ignore, ignore, p1, p2) = *f;
      s.erase(f);

      a.rem(x1, y1, x2, y2, p1);
      a_xor.rem(x1, y1, x2, y2, p2);
    } else {
      bool can = true;
      can &= 0 <= x1 && 0 <= y1 && 0 <= x2 && 0 <= y2;
      can &= x1 < n && y1 < m && x2 < n && y2 < m;
      can &= a.query(x1, y1) == a.query(x2, y2);
      can &= a_xor.query(x1, y1) == a_xor.query(x2, y2);
      cout << (can ? "Yes\n" : "No\n");
    }
  }
}
