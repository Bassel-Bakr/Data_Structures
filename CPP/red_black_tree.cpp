#include<bits/stdc++.h>
using namespace std;
using ll = long long;

template < class T > struct node {
  T x;
  int s = 1;

  node *k[2] = { };

  node(T v) {
    x = v;
  }

  ~node() {
    delete k[0];
    delete k[1];
  }

  static inline bool is_red(node * p) {
    return p ? p->s : 0;
  }

  // 1 red, 0 black
  static inline void set(node * p, int c) {
    p->s = c;
  }

  static inline void flip(node * p) {
    p->s = 1;
    p->k[0]->s = 0;
    p->k[1]->s = 0;
  }

  // 0 left, 1 right
  node *rot(int d) {
    int o = d ^ 1;
    auto c = k[o];
    k[o] = c->k[d];
    c->k[d] = this;
    swap(c->s, s);
    return c;
  }
};

template < class T > struct rbt {
  using tnode = node < T >;
  tnode *root = 0;

  ~rbt() {
    delete root;
  } tnode *insert(tnode * p, T const &x) {
    if (not p)
      return new tnode(x);
    int d = (x >= p->x);
    p->k[d] = insert(p->k[d], x);
    return fix(p, d);
  }

  void insert(T x) {
    root = insert(root, x);
    tnode::set(root, 0);
  }

  int b;
  tnode *erase(tnode * p, T const &x, tnode * fam) {
    if (not p)
      return 0;

    int d = (!fam and x >= p->x);
    int o = d ^ 1;
    auto kid = p->k[d];

    // lost & found?
    if (not kid && (fam || p->x == x)) {
      if (fam)
        swap(fam->x, p->x);

      // other kid?
      if (p->k[o]) {
        auto ret = p->k[o];
        p->k[o] = 0;
        delete p;

        tnode::set(ret, 0);
        return ret;
      } else {
        // argh black leaf baby!
        b = !tnode::is_red(p);
        delete p;
        return 0;
      }
    }

    if (p->x == x)
      fam = p;

    p->k[d] = erase(p->k[d], x, fam);
    
    return fix_erase(p, d);
  }

  void erase(T x) {
    b = 0;
    root = erase(root, x, 0);
    if (root)
      tnode::set(root, 0);
    b = 0;
  }

  tnode *fix_erase(tnode * p, int d) {
    auto kid = p->k[d];
    int o = d ^ 1;

    if (b) {
      // am I red?
      if (tnode::is_red(kid)) {
        // done yaaay
        b = 0;
        tnode::set(kid, 0);
      } else if (tnode::is_red(p->k[o])) {
        // is sibling red?
        // rotate towards me
        p = p->rot(d);
        p->k[d] = fix_erase(p->k[d], d);
        p = fix_erase(p, d);
      } else {
        // blacky
        // is close nephew red?
        if (tnode::is_red(p->k[o]->k[d])) {
          // go away!!!
          p->k[o] = p->k[o]->rot(o);
        }
        // is far nephew red?
        if (tnode::is_red(p->k[o]->k[o])) {
          // we are done!
          b = 0;
          p = p->rot(d);
          tnode::set(p->k[o], 0);
        } else {
          // both nephews are black! make bro red to even sides
          tnode::set(p->k[o], 1);
        }
      }
      return p;
    } else {
      return fix(p, d);
    }
  }

  tnode *fix(tnode * p, int d) {
    auto kid = p->k[d];
    int o = d ^ 1;
    // is a red suspect?
    if (tnode::is_red(kid)) {
      if (tnode::is_red(p->k[o])) {
        // if sibling is red just flip
        tnode::flip(p);
      } else if (tnode::is_red(kid->k[d])) {
        // single rotation
        p = p->rot(o);
      } else if (tnode::is_red(kid->k[o])) {
        // double rotation
        kid->rot(d);
        p = p->rot(o);
      }
    }
    return p;
  }

  int print(tnode * p) {
    if (not p)
      return 0;
    auto l = print(p->k[0]);
    //cout << p->x << '\n';
    auto r = print(p->k[1]);
    // black height
    assert(l == r);
    // double red
    assert(not(tnode::is_red(p)
               && (tnode::is_red(p->k[0]) || tnode::is_red(p->k[1]))));
    return not(tnode::is_red(p)) + max(l, r);
  }

  int print() {
    return print(root);
  }
};

int main()
{
  cin.tie(0);
  cin.sync_with_stdio(0);

  rbt < int >t;
  int n = 100 * 1000;

  for (int i = 0; i < n; ++i) {
    t.insert(i);
  }
  
  for (int i = 0; i < n / 2; ++i) {
    auto v = rand() % n;
    t.erase(v);
  }

  int h = t.print();

  cout << h << endl;
}
