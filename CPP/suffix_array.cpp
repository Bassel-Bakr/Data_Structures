#include<bits/stdc++.h>
using namespace std;
using ll = long long;

vector<int> suf_con(string const& s) {
  int n = s.size();
  
  if(n == 0) return {};
  
  vector<int> cnt, sa(n), tmp(n), rank[2];
  rank[0].resize(n);
  rank[1].resize(n);
  
  iota(sa.begin(), sa.end(), 0);
  for(int i = 0; i < n; ++i)
    rank[0][sa[i]] = s[sa[i]];
    
  for(int len = 0; len < n; len = max(1, 2 * len)) {
    for(auto x : sa)
      rank[1][x] = (x + len < n) ? rank[0][x + len] : 0;
      
    int m = 1 + rank[0][sa.back()];
    if(not len)
      m = 1 + *max_element(rank[0].begin(), rank[0].end());
    
    for(int r = 1; r >= 0; --r) {
      swap(sa, tmp);
      cnt.assign(m, 0);
      for(auto x : tmp)
        ++cnt[rank[r][x]];
      partial_sum(cnt.begin(), cnt.end(), cnt.begin());
      for(int i = n - 1; i >= 0; --i)
        sa[--cnt[rank[r][tmp[i]]]] = tmp[i];
    }
    
    for(int i = 0, j = 0, k = 1; i < n; i = j, ++k) {
      int r0 = rank[0][sa[i]];
      int r1 = rank[1][sa[i]];
      while(j < n && r0 == rank[0][sa[j]] && r1 == rank[1][sa[j]])
        rank[0][sa[j++]] = k;
    }    
    
    if(rank[0][sa.back()] == n) break;
  }  
  return sa;
}

vector<int> lcp_con(string const &w, vector<int> const& sa, vector<int> const& rev) {
  int n = w.size();
  vector<int> lcp(n);
  for(int i = 0, k = 0; i < n; ++i, k -= k>0) {
    int idx = rev[i];
    if(idx == n-1) {
      k = 0;
    } else {
      while(sa[idx]+k < n &&
            sa[idx+1]+k < n &&
            w[sa[idx]+k] == w[sa[idx+1]+k])
        k++;
    }
    lcp[idx] = k;
  }
  return lcp;
} 


int main()
{
  cin.tie(0);
  cin.sync_with_stdio(0);

  string s = "banana";
  
  auto sa = suf_con(s);
  
  for(int i = 0; i < int(s.size()); ++i)
    cout << sa[i] << ' ' << s.substr(sa[i], s.size()) << endl;
}
