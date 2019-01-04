use std::{cmp, mem};

fn main() {
    let mut t = Treap::new();
    let n = 1_00_000;
    for i in 0..n {
        t.insert(i);
    }
    println!("{}", t.print());
}

struct Node<T> {
    x: T,
    y: u32,
    l: Option<Box<Node<T>>>,
    r: Option<Box<Node<T>>>,
}

impl<T> Node<T> {
    fn new(x: T) -> Node<T> {
        Node {
            x,
            y: rnd(),
            l: None,
            r: None,
        }
    }

    fn new_wrapped(x: T) -> Option<Box<Node<T>>> {
        Some(Box::from(Node::new(x)))
    }
}

struct Treap<T> {
    root: Option<Box<Node<T>>>,
}

impl<T: Ord + std::fmt::Display> Treap<T> {
    fn new() -> Treap<T> {
        Treap { root: None }
    }

    fn insert(&mut self, x: T) {
        let p = mem::replace(&mut self.root, None);
        let (l, r) = Treap::split(p, &x);
        self.root = Treap::join(Treap::join(l, Node::new_wrapped(x)), r);
    }

    fn print(&self) -> u32 {
        Treap::print_subtree(&self.root)
    }

    fn print_subtree(p: &Option<Box<Node<T>>>) -> u32 {
        match *p {
            None => 0,
            Some(ref p) => {
                let (l, r) = (Treap::print_subtree(&p.l), Treap::print_subtree(&p.r));
                cmp::max(l, r) + 1
            }
        }
    }

    fn split(p: Option<Box<Node<T>>>, x: &T) -> (Option<Box<Node<T>>>, Option<Box<Node<T>>>) {
        match p {
            None => (None, None),
            Some(mut p) => {
                if *x < p.x {
                    let lf = mem::replace(&mut p.l, None);
                    let (l, r) = Treap::split(lf, x);
                    p.l = r;
                    (l, Some(p))
                } else {
                    let rg = mem::replace(&mut p.r, None);
                    let (l, r) = Treap::split(rg, x);
                    p.r = l;
                    (Some(p), r)
                }
            }
        }
    }

    fn join(l: Option<Box<Node<T>>>, r: Option<Box<Node<T>>>) -> Option<Box<Node<T>>> {
        match (l, r) {
            (None, None) => None,
            (lf, None) => lf,
            (None, rg) => rg,
            (Some(mut lf), Some(mut rg)) => {
                if lf.y > rg.y {
                    let ll = mem::replace(&mut lf.r, None);
                    mem::replace(&mut lf.r, Treap::join(ll, Some(rg)));
                    Some(lf)
                } else {
                    let rr = mem::replace(&mut rg.l, None);
                    mem::replace(&mut rg.l, Treap::join(Some(lf), rr));
                    Some(rg)
                }
            }
        }
    }
}

extern "C" {
    fn srand() -> ();
    fn rand() -> u32;
}

fn srnd() {
    unsafe { srand() }
}

fn rnd() -> u32 {
    unsafe { rand() }
}
