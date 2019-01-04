use std::mem;

fn main() {
    let n = 100usize;
    let mut uf = UnionFind::new(n);
}

struct UnionFind {
    fa: Vec<isize>,
}

impl UnionFind {
    fn new(n: usize) -> UnionFind {
        UnionFind {
            fa: vec![-1isize; n],
        }
    }

    fn find(&mut self, mut x: usize) -> usize {
        let mut p = x;
        while !self.fa[p].is_negative() {
            p = self.fa[p] as usize;
        }
        while !self.fa[x].is_negative() {
            x = mem::replace(&mut self.fa[x], p as isize) as usize;
        }
        p
    }

    fn union(&mut self, x: usize, y: usize) -> bool {
        let x = self.find(x);
        let y = self.find(y);
        if x == y {
            false
        } else {
            self.fa[x] += self.fa[y];
            self.fa[y] = x as isize;
            true
        }
    }

    fn size(&mut self, x: usize) -> usize {
        let p = self.find(x);
        (-self.fa[p]) as usize
    }
}
