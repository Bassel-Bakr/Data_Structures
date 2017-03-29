from random import randrange as rand

class Node:
	def __init__(self, x):
		self.x = x
		self.y = rand(1000000000)
		self.l = self.r = None

class Treap:
	def __init__(self):
		self.root = None

	# join two trees O(log n)
	def join(self, l, r):
		if not l: return r
		if not r: return l

		if l.y < r.y:
			l.r = self.join(l.r, r)
			return l
		else:
			r.l = self.join(l, r.l)
			return r

	# split into two trees O(log n)
	def split(self, x, p):
		if not p:
			return (None, None)

		if x < p.x:
			l, r = self.split(x, p.l)
			p.l = r
			return (l, p);
		else:
			l, r = self.split(x, p.r)
			p.r = l
			return (p, r)

	# O(log n)
	def insert(self, x):
		l, r = self.split(x, self.root)
		self.root = self.join(l, self.join(Node(x), r))

	# delete one element O(log n)
	def delete(self, x):
		l, r = self.split(x, self.root)
		l2, r2 = self.split(x-1, l)
		l = self.join(l2, self.join(r2.l, r2.r) if r2 else None)
		self.root = self.join(l, r)

	# delete range of elements O(log n)
	def delete(self, L, R):
		l, r = self.split(R, self.root)
		l2, r2 = self.split(L-1, l)
		self.root = self.join(l2, r)

	# O(log n)
	def find(self, x):
		p = self.root
		while p:
			if x == p.x:
				return True
			
			if x < p.x:
				p = p.l
			else:
				p = p.r
		return False

	# O(n)
	def inorder(self, p, level, view):
		if not p: return level
		l = self.inorder(p.l, level + 1, view)
		if view: print(p.x)
		r = self.inorder(p.r, level + 1, view)
		return max(l, r)

	# O(n)
	def disply(self, view = True):
		return self.inorder(self.root, 0, view)

t = Treap()
for i in range(10000):
	t.insert(i)

t.delete(6, 9994)
print("Levels: %d" % t.disply())
