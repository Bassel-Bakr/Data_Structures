#![allow(dead_code)]
#![allow(unused_variables, unused_imports)]

use std::cell::RefCell;
use std::cmp;
use std::io::*;
use std::rc::{Rc, Weak};
use std::{mem, mem::ManuallyDrop};

fn main() {
    // let input = &mut Input::new();
    // let output = &mut BufWriter::new(stdout());
    let mut t = Tree::new();
    for i in 0..1000_000 {
        t.insert(unsafe { rand() });
    }
}

extern "C" {
    fn rand() -> i32;
}

type Branch<T> = Option<Rc<ManuallyDrop<RefCell<Node<T>>>>>;
type Parent<T> = Option<Weak<ManuallyDrop<RefCell<Node<T>>>>>;

struct Node<T> {
    x: T,
    l: Branch<T>,
    r: Branch<T>,
    p: Parent<T>,
}

impl<T> Node<T> {
    fn new(x: T) -> Self {
        Node {
            x: x,
            l: None,
            r: None,
            p: None,
        }
    }

    fn new_branch(x: T) -> Branch<T> {
        Some(Rc::new(ManuallyDrop::new(RefCell::new(Node::new(x)))))
    }
}

struct Tree<T: std::fmt::Display> {
    root: Branch<T>,
}

impl<T: std::fmt::Display> Drop for Tree<T> {
    fn drop(&mut self) {
        let root = mem::take(&mut self.root);
        let mut a = Vec::new();
        a.push(root);

        while let Some(mut p) = a.pop() {
            let l = mem::take(&mut p.as_ref().unwrap().borrow_mut().l);
            let r = mem::take(&mut p.as_ref().unwrap().borrow_mut().r);

            let mut x = Rc::get_mut(p.as_mut().unwrap());
            if x.is_some() {
                unsafe {
                    ManuallyDrop::drop(x.as_mut().unwrap());
                }
            } else {
                a.push(p);
            }

            if l.is_some() {
                a.push(l);
            }

            if r.is_some() {
                a.push(r);
            }
        }
    }
}

impl<T: PartialOrd + std::fmt::Display> Tree<T> {
    fn new() -> Self {
        Tree { root: None }
    }

    fn insert(&mut self, x: T) {
        let (l, r) = Self::split(mem::take(&mut self.root), &x);
        let node = Node::new_branch(x);
        {
            let mut m = node.as_ref().unwrap().borrow_mut();
            if l.is_some() {
                l.as_ref().unwrap().borrow_mut().p = Some(Rc::downgrade(node.as_ref().unwrap()));
                m.l = l;
            }
            if r.is_some() {
                r.as_ref().unwrap().borrow_mut().p = Some(Rc::downgrade(node.as_ref().unwrap()));
                m.r = r;
            }
        }
        self.root = node;
    }

    fn split(root: Branch<T>, x: &T) -> (Branch<T>, Branch<T>) {
        if root.is_none() {
            (None, None)
        } else {
            let mut p = Rc::clone(root.as_ref().unwrap());
            let mut left;

            loop {
                if x < &p.borrow().x {
                    left = false;
                    if p.borrow().l.is_some() {
                        p = {
                            let b = p.borrow();
                            Rc::clone(b.l.as_ref().unwrap())
                        }
                    } else {
                        break;
                    }
                } else {
                    left = true;
                    if p.borrow().r.is_some() {
                        p = {
                            let b = p.borrow();
                            Rc::clone(b.r.as_ref().unwrap())
                        }
                    } else {
                        break;
                    }
                }
            }

            if left {
                let l = Some(p);
                Self::splay(&l);
                let r = mem::take(&mut l.as_ref().unwrap().borrow_mut().r);
                if r.is_some() {
                    r.as_ref().unwrap().borrow_mut().p = None;
                }

                (l, r)
            } else {
                let r = Some(p);
                Self::splay(&r);
                let l = mem::take(&mut r.as_ref().unwrap().borrow_mut().l);
                if l.is_some() {
                    l.as_ref().unwrap().borrow_mut().p = None;
                }

                (l, r)
            }
        }
    }

    fn join(l: Branch<T>, r: Branch<T>) -> Branch<T> {
        match (l.as_ref(), r.as_ref()) {
            (None, _) => r,
            (_, None) => l,
            _ => {
                // splay rightmost node in `l`
                let mut p = Rc::clone(l.as_ref().unwrap());
                while p.borrow().r.is_some() {
                    p = {
                        let pm = p.borrow();
                        Rc::clone(pm.r.as_ref().unwrap())
                    }
                }

                r.as_ref().unwrap().borrow_mut().p = Some(Rc::downgrade(&p));
                let root = Some(p);
                Self::splay(&root);
                root.as_ref().unwrap().borrow_mut().r = r;
                root
            }
        }
    }

    fn is_left(c: &Branch<T>) -> bool {
        let me = c.as_ref().unwrap().borrow();
        if me.p.is_some() {
            let pp = me.p.as_ref().unwrap().upgrade();
            if pp.is_none() {
                return false;
            }
            let up = pp.as_ref().unwrap().borrow();
            up.l.is_some() && Rc::ptr_eq(c.as_ref().unwrap(), up.l.as_ref().unwrap())
        } else {
            false
        }
    }

    fn splay(c: &Branch<T>) -> &Branch<T> {
        while c.as_ref().unwrap().borrow().p.is_some() {
            let p = c.as_ref().unwrap().borrow().p.as_ref().unwrap().upgrade();
            let rot_parent = {
                let g = &p.as_ref().unwrap().borrow().p;

                g.is_some() && (Self::is_left(c) == Self::is_left(&p))
            };

            if rot_parent {
                Self::rot(&p);
            }

            Self::rot(c);
        }
        c
    }

    fn rot(c: &Branch<T>) -> &Branch<T> {
        // get the parent
        let mut p = c.as_ref().unwrap().borrow().p.as_ref().unwrap().upgrade();

        // where is grandpa?
        let g = {
            let node = p.as_ref().unwrap().borrow();

            match node.p.as_ref() {
                Some(weak_g) => weak_g.upgrade(),
                _ => None,
            }
        };

        let is_left_parent = Self::is_left(&p);

        // ignore the grandpa now
        let is_left_child = Self::is_left(c);

        if is_left_child {
            let mut cm = c.as_ref().unwrap().borrow_mut();
            {
                let mut pm = p.as_ref().unwrap().borrow_mut();
                let kid = mem::take(&mut cm.r);
                if kid.is_some() {
                    kid.as_ref().unwrap().borrow_mut().p = Some(Rc::downgrade(p.as_ref().unwrap()));
                }
                cm.p = mem::replace(&mut pm.p, Some(Rc::downgrade(c.as_ref().unwrap())));
                if g.is_some() {
                    if is_left_parent {
                        g.as_ref().unwrap().borrow_mut().l = mem::take(&mut pm.l);
                    } else {
                        g.as_ref().unwrap().borrow_mut().r = mem::take(&mut pm.l);
                    }
                }
                pm.l = kid;
            }
            cm.r = mem::take(&mut p);
        } else {
            let mut cm = c.as_ref().unwrap().borrow_mut();
            {
                let mut pm = p.as_ref().unwrap().borrow_mut();
                let kid = mem::take(&mut cm.l);
                if kid.is_some() {
                    kid.as_ref().unwrap().borrow_mut().p = Some(Rc::downgrade(p.as_ref().unwrap()));
                }
                cm.p = mem::replace(&mut pm.p, Some(Rc::downgrade(c.as_ref().unwrap())));
                if g.is_some() {
                    if is_left_parent {
                        g.as_ref().unwrap().borrow_mut().l = mem::take(&mut pm.r);
                    } else {
                        g.as_ref().unwrap().borrow_mut().r = mem::take(&mut pm.r);
                    }
                }
                pm.r = kid;
            }
            cm.l = mem::take(&mut p);
        }

        c
    }
}

//{{{
struct Input {
    buf: Vec<String>,
}

impl Input {
    fn new() -> Self {
        Input { buf: Vec::new() }
    }

    fn next<T: std::str::FromStr>(&mut self) -> T {
        if self.buf.is_empty() {
            self.buf = self
                .next_line()
                .split_whitespace()
                .rev()
                .map(String::from)
                .collect();
        }

        self.buf.pop().unwrap().parse().ok().expect("err")
    }

    fn next_line(&mut self) -> String {
        let mut line = String::new();
        stdin().read_line(&mut line).expect("read err");
        line
    }
} //}}}
