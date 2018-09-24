#include <bits/stdc++.h>

using namespace std;

using ll = long long;

template <class T>
struct tnode {
  T x;
  tnode *p;
  tnode *l;
  tnode *r;

  tnode() { l = r = p = 0; }

  tnode(T v) : tnode() { x = v; }

  tnode *upd() { return this; }
};

template <class T, class F = less<T>>
struct splay_tree {
  F f;
  using node = tnode<T>;

  node *root;

  deque<node> store;
  vector<node *> empty;

  node *new_node(T const &x) {
    if (empty.empty()) {
      store.emplace_back(x);
      return &store.back();
    } else {
      auto v = empty.back();
      empty.pop_back();
      *v = node(x);
      return v;
    }
  }

  void delete_node(node *p) { empty.emplace_back(p); }

  splay_tree() { root = 0; }

  ~splay_tree() {}

  node *rot(node *c) {
    auto p = c->p;
    auto g = p->p;
    if (p->l == c) {  // right
      p->l = c->r;
      if (p->l) p->l->p = p;
      c->r = p;
    } else {  // left
      p->r = c->l;
      if (p->r) p->r->p = p;
      c->l = p;
    }
    if (g) {
      if (g->l == p)
        g->l = c;
      else
        g->r = c;
    }
    p->p = c;
    c->p = g;
    p->upd();
    return c->upd();
  }

  node *splay(node *c) {
    while (c->p) {
      auto p = c->p;
      auto g = p->p;
      if (g && (g->l == p) == (p->l == c)) rot(p);
      rot(c);
    }
    return c->upd();
  }

  pair<node *, node *> split(node *p, T const x) {
    node *l = 0;
    node *r = 0;
    node *last = 0;
    node *last_valid = 0;
    while (p) {
      last = p;
      if (f(x, p->x)) {  // x < p->x
        p = p->l;
      } else {
        last_valid = p;
        p = p->r;
      }
    }
    if (last_valid) {
      splay(last_valid);
      l = last_valid;
      r = l->r;
      l->r = 0;
      l->upd();
      if (r) r->p = 0;
    } else if (last) {
      r = splay(last);
    }
    return make_pair(l, r);
  }

  node *join(node *l, node *r) {
    if (!l || !r) return l ?: r;
    while (l->r) l = l->r;
    splay(l);
    l->r = r;
    r->p = l;
    return l->upd();
  }

  void insert(T const &x) {
    auto [l, r] = split(root, x);
    root = join(l, join(new_node(x), r));
  }

  void erase(T const &x) {
    auto [l, r] = split(root, x);
    if (l && l->x == x) {
      auto lf = l->l;
      auto rg = l->r;
      if (lf) lf->p = 0;
      if (rg) rg->p = 0;
      delete_node(l);
      l = join(lf, rg);
    }
    root = join(l, r);
  }

  void print() {
    if (root) {
      vector<pair<node *, bool>> stk;
      stk.emplace_back(root, false);
      while (stk.size()) {
        auto state = stk.back();
        auto p = state.first;
        if (not state.second) {
          stk.back().second = true;
          if (p->l) stk.emplace_back(p->l, false);
        } else {
          cout << p->x << '\n';
          stk.pop_back();
          if (p->r) stk.emplace_back(p->r, false);
        }
      }
    }
  }
};

int main() {
  cin.tie(0);
  cin.sync_with_stdio(0);

  int n;
  cin >> n;
  splay_tree<int> t;
  for (int i = 0; i < n; ++i) {
    t.insert(i);
  }

  // t.print();
}
