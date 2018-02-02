#include <bits/stdc++.h>

using namespace std;
using ll = long long;

int const N = 1 << 21;

ll sum[N];
int mx1[N];
int mx2[N];
int cnt[N];

struct segment_tree
{
	int n;
	int ql, qr;

	  segment_tree(vector < int >const &a)
	{
		n = a.size();
		build(1, 1, n, a);
	}

	inline void calc(int i)
	{
		int lf = 2 * i;
		int rg = lf + 1;

		cnt[i] = 0;
		sum[i] = sum[lf] + sum[rg];
		mx1[i] = max(mx1[lf], mx1[rg]);
		mx2[i] = max(mx2[lf], mx2[rg]);

		if (mx1[i] == mx1[lf])
			cnt[i] += cnt[lf];
		else
			mx2[i] = max(mx2[i], mx1[lf]);

		if (mx1[i] == mx1[rg])
			cnt[i] += cnt[rg];
		else
			mx2[i] = max(mx2[i], mx1[rg]);
	}

	void build(int i, int l, int r, vector < int >const &a)
	{
		if (l == r)
		{
			cnt[i] = 1;
			mx2[i] = -10;
			sum[i] = mx1[i] = a[l];
		}
		else
		{
			int m = (l + r) / 2;
			build(2 * i, l, m, a);
			build(2 * i + 1, m + 1, r, a);
			calc(i);
		}
	}

	void push(int i, int l, int r)
	{
		if (l == r)
			return;

		int lf = 2 * i;
		int rg = lf + 1;

		if (mx1[lf] > mx1[i])
		{
			sum[lf] -= (mx1[lf] - mx1[i]) * ll(cnt[lf]);
			mx1[lf] = mx1[i];
		}

		if (mx1[rg] > mx1[i])
		{
			sum[rg] -= (mx1[rg] - mx1[i]) * ll(cnt[rg]);
			mx1[rg] = mx1[i];
		}
	}

	void update(int i, int l, int r, int x)
	{
		if (l > r)
			return;

		push(i, l, r);

		if (mx1[i] <= x)
			return;

		if (ql <= l && r <= qr && mx2[i] < x && x < mx1[i])
		{
			sum[i] -= (mx1[i] - x) * ll(cnt[i]);
			mx1[i] = x;
		}
		else
		{
			if (l == r)
				return;
			int m = (l + r) / 2;
			if (ql <= m)
				update(2 * i, l, m, x);
			if (m < qr)
				update(2 * i + 1, m + 1, r, x);
			calc(i);
		}
	}

	void update(int l, int r, int x)
	{
		ql = l;
		qr = r;
		update(1, 1, n, x);
	}


	ll query_sum(int i, int l, int r)
	{
		push(i, l, r);

		if (ql <= l && r <= qr)
		{
			return sum[i];
		}
		else
		{
			int m = (l + r) / 2;
			ll x = 0;
			if (ql <= m)
				x += query_sum(2 * i, l, m);
			if (m < qr)
				x += query_sum(2 * i + 1, m + 1, r);
			return x;
		}
	}

	ll query_sum(int l, int r)
	{
		ql = l;
		qr = r;
		return query_sum(1, 1, n);
	}


	int query_max(int i, int l, int r)
	{
		push(i, l, r);

		if (ql <= l && r <= qr)
		{
			return mx1[i];
		}
		else
		{
			int m = (l + r) / 2;
			int x = 0;
			if (ql <= m)
				x = max(x, query_max(2 * i, l, m));
			if (m < qr)
				x = max(x, query_max(2 * i + 1, m + 1, r));
			return x;
		}
	}

	int query_max(int l, int r)
	{
		ql = l;
		qr = r;
		return query_max(1, 1, n);
	}
};
