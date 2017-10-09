struct fenwick {
  int n;
  vector<ll> a, b;

  fenwick(int s) {
    n = s;
    a.assign(n+1, 0);
    b.assign(n+1, 0);
  }

  void update(vector<ll>& bit, int idx, ll v) {
    for(int i = idx; i <= n; i += (i&-i))
      bit[i] += v;
  }

  void update(int l, int r, ll v) {
    update(a, l, v);
    update(a, r+1, -v);
    update(b, l, -v*(l-1));
    update(b, r+1, v*r);
  }

  ll query(vector<ll> const& bit, ll idx) {
    ll sum = 0;
    for(int i = idx; i > 0; i -= (i&-i))
      sum += bit[i];
    return sum;
  }

  ll query(int idx) {
    return query(a, idx) * idx + query(b, idx);
  }

  ll query(int l, int r) {
    return query(r) - query(l-1);
  }
};
