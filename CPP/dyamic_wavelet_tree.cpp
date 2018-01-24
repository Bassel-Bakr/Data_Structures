#include <bits/stdc++.h>
using namespace std;

struct node {
  int x, y, s;
  node *p = 0;
  node *l = 0;
  node *r = 0;

  node(int v) {
    x = v;
    y = v;
    s = 1;
  }

  void upd() {
    s = 1;
    y = x;
    if(l) {
      y += l->y;
      s += l->s;
    }

    if(r) {
      y += r->y;
      s += r->s;
    }
  }

  int left_size() {
    return l ? l->s : 0;
  }
};

struct splay_tree {
  node *root = 0;

  void rot(node *c) {
    auto p = c->p;
    auto g = p->p;

    if(g)
      (g->l == p ? g->l : g->r) = c;

    if(p->l == c) {
      p->l = c->r;
      c->r = p;
      if(p->l) p->l->p = p;
    } else {
      p->r = c->l;
      c->l = p;
      if(p->r) p->r->p = p;
    }

    p->p = c;
    c->p = g;

    p->upd();
    c->upd();
  }

  void splay(node *c) {
    while(c->p) {
      auto p = c->p;
      auto g = p->p;

      if(g) rot((g->l == p) == (p->l == c) ? p : c);
      rot(c);
    }
    c->upd();
    root = c;
  }

  node* join(node *l, node *r) {
    if(not l) return r;
    if(not r) return l;

    while(l->r) l = l->r;

    splay(l);
    r->p = l;
    l->r = r;
    l->upd();

    return l;
  }

  pair<node*,node*> split(node *p, int idx) {
    if(not p)
      return make_pair(nullptr, nullptr);

    if(idx < 0)
      return make_pair(nullptr, p);

    if(idx >= p->s)
      return make_pair(p, nullptr);

    for(int lf = p->left_size(); idx != lf; lf = p->left_size()) {
      if(idx < lf)
        p = p->l;
      else
        p = p->r, idx -= lf+1;
    }

    splay(p);

    node *l = p;
    node *r = p->r;
    if(r) {
      l->r = r->p = 0;
      l->upd();
    }
    return make_pair(l, r);
  }

  node* get(int idx) {
    auto p = root;
    for(int lf = p->left_size(); idx != lf; lf = p->left_size()) {
      if(idx < lf)
        p = p->l;
      else
        p = p->r, idx -= lf+1;
    }

    splay(p);

    return p;
  }

  int insert(int idx, int x) {
    node *l, *r;
    tie(l, r) = split(root, idx-1);
    int v = l ? l->y : 0;
    root = join(l, join(new node(x), r));
    return v;
  }

  void erase(int idx) {
    node *l, *r;
    tie(l, r) = split(root, idx);
    root = join(l->l, r);
    delete l;
  }

  void append(int x) {
    root = join(root, new node(x));
  }

  int rank(int idx) {
    node *l, *r;
    tie(l, r) = split(root, idx);
    int x = (l && l->l ? l->l->y : 0);
    root = join(l, r);
    return x;
  }

  int operator[](int idx) {
    return rank(idx);
  }

  ~splay_tree() {
    if(!root)
      return;

    vector<node*> nodes {root};
    while(nodes.size()) {
      auto u = nodes.back();
      nodes.pop_back();
      if(u->l) nodes.emplace_back(u->l);
      if(u->r) nodes.emplace_back(u->r);
      delete u;
    }
  }
};

struct wavelet {
  int A, Z;
  wavelet *l = 0;
  wavelet *r = 0;
  splay_tree b;

  wavelet(int L, int R) {
    A = L;
    Z = R;
    b.insert(0, 0);
  }

  ~wavelet() {
    delete l;
    delete r;
  }

  void append(int x) {
    if(A >= Z)
      return;

    int M = (A + Z) / 2;

    if(x <= M) {
      b.append(1);
      l = l ?: new wavelet(A, M);
      l->append(x);
    } else {
      b.append(0);
      r = r ?: new wavelet(M+1, Z);
      r->append(x);
    }
  }

  void insert(int idx, int x) {
    if(A >= Z)
      return;

    int M = (A + Z) / 2;    

    if(x <= M) {
      l = l ?: new wavelet(A, M);
      l->insert(b.insert(idx, 1), x);
    } else {
      r = r ?: new wavelet(M+1, Z);
      r->insert(idx - b.insert(idx, 0), x);
    }
  }

  void erase(int idx) {
    if(A == Z)
      return;

    auto p = b.get(idx);
    int lf = p->y;
    b.erase(idx);
    if(p->x == 1)
      l->erase(lf);
    else
      r->erase(idx-lf);
  }

  int kth(int L, int R, int k) {
    if(A == Z)
      return A;

    int x = b.rank(L);
    int y = b.rank(R);

    if(k <= y-x)
      return l->kth(x, y, k);
    else
      return r->kth(L-x, R-y, k-(y-x));
  }
};

int main() {
  cin.tie(0);
  cin.sync_with_stdio(0);

  wavelet t(0, 1e9);

  int n = 100000;
  for(int i = 0; i < n; ++i)
    t.insert(i, i+1);

  cout << t.kth(0, n, 15683);
}
