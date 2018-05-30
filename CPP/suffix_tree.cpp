#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int inf = 1000 * 1000 * 1000;

int tot;
int step;
int eds;
int suffs;

struct node;

struct edge {
  int l, r;
  node *dest;

  edge(int ls, int rs, node *to) {
    l = ls;
    r = rs;
    dest = to;
    eds++;
  }

  edge(edge *other) : edge(other->l, other->r, other->dest) {}
};

struct node {
  node *suffix;
  map<char, edge *> kids;

  node() {
    tot++;
    suffix = 0;
  }
};

struct suffix_tree {
  int rem = 0;
  int ac_pos = 0;
  edge *ac_edge = 0;
  node *ac_node = 0;

  string s;
  node *root = 0;

  suffix_tree(string const &ss) {
    s = ss;
    root = new node();
    ac_node = root;
    ac_edge = 0;
    ac_pos = 0;
    for (int i = 0; i < s.size(); ++i) add(i, s[i]);
  }

  ~suffix_tree() {
    vector<node *> del{root};
    while (del.size()) {
      auto u = del.back();
      del.pop_back();
      for (auto [ignore, edge] : u->kids) {
        if (edge->dest) del.emplace_back(edge->dest);
        delete edge;
      }
      delete u;
    }
  }

  void add(int i, char c) {
    rem++;
    node *prev = 0;
    while (rem) {
      step++;
      if (ac_edge) {
        if (c == s[ac_edge->l + ac_pos + 1]) {
          ac_pos++;
          if (ac_edge->l + ac_pos + 1 > ac_edge->r) {
            ac_node = ac_edge->dest;
            ac_edge = 0;
          }
          break;
        } else {
          rem--;
          auto ec = new edge(ac_edge);
          ec->l += ac_pos + 1;
          ac_edge->r += ec->l;

          auto nod = new node();
          ac_edge->dest = nod;
          nod->kids[c] = new edge(i, inf, 0);
          nod->kids[s[ac_edge->l + ac_pos]] = ec;

          if (prev) {
            suffs++;
            prev->suffix = nod;
          }
          prev = nod;

          if (ac_node->suffix) {
            ac_node = ac_node->suffix;
            ac_edge = ac_node->kids[s[ac_edge->l]];
          } else if (ac_node != root) {
            ac_node = root;
            ac_edge = ac_node->kids[s[ac_edge->l + ac_pos]];
          } else {
            ac_edge = (ac_pos > 0 ? ac_node->kids[s[ac_edge->l + --ac_pos]] : 0);
          }
        }
      } else {
        auto &e = ac_node->kids[c];
        if (e) {
          ac_edge = e;
          ac_pos = 0;
          if (ac_edge->l + ac_pos + 1 > ac_edge->r) {
            ac_node = ac_edge->dest;
            ac_edge = 0;
          }
          break;
        } else {
          rem--;
          e = new edge(i, inf, 0);
          if (ac_node->suffix) {
            ac_node = ac_node->suffix;
            ac_edge = 0;
          } else {
            ac_node = root;
            ac_edge = 0;
          }
        }
      }
    }
  }
};

int main() {
  string s = "abcabxabcd$";

  suffix_tree st(s);

  cout << "Nodes: " << tot << "\n"
       << "Edges: " << eds << "\n"
       << "Links: " << suffs << "\n"
       << "Steps: " << step + 1 << "\n";
}
