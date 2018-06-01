#include <bits/stdc++.h>
using ll = long long;
using namespace std;

struct node {
  int len;
  node *suf;
  bool terminal = false;
  map<char, node *> kids;

  node(int lenn, node *suff) {
    len = lenn;
    suf = suff;
  }

  node(node *src) {
    len = src->len;
    suf = src->suf;
    kids = src->kids;
  }
};

struct sa {
  node *last;
  node *root;
  vector<node *> stash;

  sa() {
    root = new node(0, 0);
    root->suf = root;
    last = root;
  }

  sa(string const &s) : sa() {
    for (char c : s) add(c);
  }

  ~sa() {
    for (auto x : stash) delete x;
  }

  void add(char c) {
    auto cur = new node(last->len + 1, root);
    stash.emplace_back(cur);
    auto p = last;
    while (p->kids.find(c) == p->kids.end()) {
      p->kids[c] = cur;
      p = p->suf;
    }
    auto q = p->kids[c];
    if (q != cur) {
      if (p->len + 1 == q->len) {
        cur->suf = q;
      } else {
        auto clone = new node(q);
        stash.emplace_back(clone);
        clone->suf = root;
        clone->len = p->len + 1;
        cur->suf = q->suf = clone;
        while (p->kids[c] == q) {
          p->kids[c] = clone;
          p = p->suf;
        }
      }
    }
    last = cur;
  }

  void mark_ends() {
    auto p = last;
    while (not p->terminal) {
      p->terminal = true;
      p = p->suf;
    }
  }

  bool find(string const &s) {
    auto p = root;
    for (char c : s) {
      auto f = p->kids.find(c);
      if (f == p->kids.end()) return false;
      p = f->second;
    }
    return true;
  }
};

int main() {
  cin.tie(0);
  cin.sync_with_stdio(0);

  sa a("basselbakr");
}
