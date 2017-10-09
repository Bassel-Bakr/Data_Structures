template<class T>
struct fenwick {
  int n;
  vector<T> a, b;

  fenwick(int s) {
    n = s;
    a.assign(n+1, 0);
    b.assign(n+1, 0);
  }

  void update(vector<T>& bit, int idx, T v) {
    for(int i = idx; i <= n; i += (i&-i))
      bit[i] += v;
  }

  void update(int l, int r, T v) {
    update(a, l, v);
    update(a, r+1, -v);
    update(b, l, -v*(l-1));
    update(b, r+1, v*r);
  }

  T query(vector<T> const& bit, T idx) {
    T sum = 0;
    for(int i = idx; i > 0; i -= (i&-i))
      sum += bit[i];
    return sum;
  }

  T query(int idx) {
    return query(a, idx) * idx + query(b, idx);
  }

  T query(int l, int r) {
    return query(r) - query(l-1);
  }
};
