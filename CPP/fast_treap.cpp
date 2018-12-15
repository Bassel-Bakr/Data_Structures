template <class T, class F = less<T>>
struct treap {
  template <class R>
  struct tnode {
    R x;
    int y, s;
    tnode *l, *r;
    tnode(R const &v, int rnd) {
      x = v;
      y = rnd;
      s = 1;
      l = r = 0;
    }
    ~tnode() {
      delete l;
      delete r;
    }
    inline tnode *upd() {
      s = 1;
      if (l) s += l->s;
      if (r) s += r->s;
      return this;
    }
  };

  using node = tnode<T>;
  F f;
  node *root;
  mt19937 rand;

  treap() {
    root = 0;
    rand.seed(chrono::steady_clock::now().time_since_epoch().count());
  }

  ~treap() { delete root; }

  node *join(node *l, node *r) {
    if (not l) return r;
    if (not r) return l;
    if (l->y > r->y) {
      l->r = join(l->r, r);
      return l->upd();
    } else {
      r->l = join(l, r->l);
      return r->upd();
    }
  }

  node *insert(node *p, node *x) {
    if (not p) return x;
    if (f(x->x, p->x)) {
      p->l = insert(p->l, x);
      auto c = p->l;
      if (c->y > p->y) {
        p->l = c->r;
        c->r = p->upd();
        p = c;
      }
    } else {
      p->r = insert(p->r, x);
      auto c = p->r;
      if (c->y > p->y) {
        p->r = c->l;
        c->l = p->upd();
        p = c;
      }
    }
    return p->upd();
  }

  node *erase(node *p, T const &x) {
    if (not p) return 0;
    if (f(x, p->x)) {
      p->l = erase(p->l, x);
      p->upd();
    } else if (f(p->x, x)) {
      p->r = erase(p->r, x);
      p->upd();
    } else {
      auto kids = join(p->l, p->r);
      p->l = p->r = 0;
      delete p;
      p = kids;
    }
    return p;
  }

  void insert(T const &x) { root = insert(root, new node(x, rand())); }
  void erase(T const &x) { root = erase(root, x); }

  int print(node *p) {
    if (not p) return 0;
    auto l = print(p->l);
    cout << p->x << '\n';
    auto r = print(p->r);
    return max(l, r) + 1;
  }

  int print() { return print(root); }
};
