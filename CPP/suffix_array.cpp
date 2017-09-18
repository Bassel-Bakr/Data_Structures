#include <bits/stdc++.h>

using namespace std;

struct suffix_array {  
  int n;
  vector<int> sa, lcp, rev, rad;
  
  struct suffix {
  	int i, x, y;
  };

	void radix_sort(vector<suffix>& suf, vector<suffix>& tmp) {
		int mx = 0;
		for (int i = 0; i < n; ++i)
			  mx = max({mx, suf[i].x, suf[i].y});

		rad.resize(mx+1);
		fill(rad.begin(), rad.end(), 0);
		for (int i = 0; i < n; ++i)
			++rad[suf[i].y];
		partial_sum(rad.begin(), rad.end(), rad.begin());
		for (int i = n - 1; i >= 0; --i)
			tmp[--rad[suf[i].y]] = suf[i];

		fill(rad.begin(), rad.end(), 0);
		for (int i = 0; i < n; ++i)
			++rad[tmp[i].x];
		partial_sum(rad.begin(), rad.end(), rad.begin());
		for (int i = n - 1; i >= 0; --i)
			suf[--rad[tmp[i].x]] = tmp[i];
	}

	suffix_array(string const &w) {
		n = w.size();
		rev.resize(n);
		vector<suffix> suf(n), tmp(n);

		for (int i = 0; i < n; ++i)
			suf[i] = {i, w[i], 0};

		int p = 1;
		do {
			radix_sort(suf, tmp);
			int x = suf[0].x;
			suf[0].x = 1;
			for (int i = 1; i < n; ++i) {
				int x2 = suf[i].x, y2 = suf[i].y;
				suf[i].x = suf[i - 1].x + (x != x2 || suf[i].y != suf[i - 1].y);
				x = x2;
			}
			// update rev
			for (int i = 0; i < n; ++i)
				rev[suf[i].i] = i;
			// update minor
			for (int i = 0; i < n; ++i) {
				int next = suf[i].i + p;
				suf[i].y = next < n ? suf[rev[next]].x : 0;
			}
			p <<= 1;
		}
		while (suf[n - 1].x != n);

		sa.resize(n);
		for (int i = 0; i < n; ++i)
			sa[i] = suf[i].i;
	}

	void build_lcp(string const &w) {
	  lcp.resize(n);
	  for(int i = 0, k = 0; i < n; ++i, k -= k>0) {
	    int idx = rev[i];
	    if(idx == n-1) {
	      k = 0;
	    } else {
	      while(sa[idx]+k < n && sa[idx+1]+k < n &&
			w[sa[idx]+k] == w[sa[idx+1]+k])
			k++;
	    }
	    lcp[idx] = k;
	  }
	}	

	int operator[](int i) {
	  return sa[i];
	}
};

int main() {
	cin.tie(0);
	cin.sync_with_stdio(0);

	string s;
	cin >> s;
	int n = s.size();
	suffix_array sa(s);
	sa.build_lcp(s);
	for(int i = 0; i < n; ++i)
	  cout << sa.lcp[i] << " " << &s[sa[i]] << "\n";
}
