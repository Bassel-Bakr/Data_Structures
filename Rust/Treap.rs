#![allow(dead_code)]

fn main() {
    let n = 100 * 1000;
    let mut t = data_structures::Treap::new();

    for i in 0..n {
        t.insert(i);
    }

    let h = t.print();

    for i in 0..n {
        t.delete(&i);
    }

    println!("Height = {}", h);
}

mod data_structures {
    use std::cmp;
    use std::mem;

    extern "C" {
        fn rand() -> i32;
    }

    type Branch<T> = Option<Box<Node<T>>>;

    trait Quirky<'a, T> {
        fn reffy(&'a self) -> &'a T;
        fn mutty(&'a mut self) -> &'a mut T;
    }

    impl<'a, T> Quirky<'a, T> for Option<T> {
        fn reffy(&'a self) -> &'a T {
            self.as_ref().unwrap()
        }

        fn mutty(&'a mut self) -> &'a mut T {
            self.as_mut().unwrap()
        }
    }

    struct Node<T> {
        x: T,
        y: i32,
        l: Branch<T>,
        r: Branch<T>
    }

    impl<T: Default> Default for Node<T> {
        fn default() -> Self {
            Node {
                x: Default::default(),
                y: unsafe { rand() },
                l: None,
                r: None 
            }
        }
    }

    impl<T: Default> Node<T> {
        fn new(x: T) -> Self {
            Node { x: x, ..Default::default() }
        }

        fn new_branch(x: T) -> Branch<T> {
            Some(Box::new(Node::new(x)))
        }
    }

    pub struct Treap<T> {
        root: Branch<T>
    }

    impl<T: Default + PartialOrd + std::fmt::Display> Treap<T> {
        pub fn new() -> Self {
            Treap { root: None }
        }

        pub fn insert(&mut self, x: T) {
            let (mut l, mut r) = Self::split(&mut self.root, &x);
            self.root = Self::join(&mut l, &mut Self::join(&mut Node::new_branch(x), &mut r));
        }

        pub fn delete(&mut self, x: &T) {
            let (mut l, mut r) = Self::split(&mut self.root, x);
            l = Self::delete_rightmost(&mut l, x);
            self.root = Self::join(&mut l, &mut r);
        }

        fn delete_rightmost(root: &mut Branch<T>, x: &T) -> Branch<T> {
            if root.is_none() {
                None
            } else {
                let mut p = mem::take(root);
                let mut r = mem::take(&mut p.mutty().r);
                if r.is_none() {
                    if &p.reffy().x == x {
                        None
                    } else {
                        p    
                    }
                } else {
                    p.mutty().r = Self::delete_rightmost(&mut r, x);
                    p
                }
            }
        }

        fn join(lf: &mut Branch<T>, rg: &mut Branch<T>) -> Branch<T> {
            let (mut l, mut r) = (mem::take(lf), mem::take(rg));

            let (l_ref, r_ref) = (l.as_ref(), r.as_ref());

            match (l_ref, r_ref) {
                (None, _) => r,
                (_, None) => l,
                _ => if l_ref.unwrap().y > r_ref.unwrap().y {
                    l.mutty().r = Self::join(&mut l.mutty().r, &mut r);
                    l
                } else {
                    r.mutty().l = Self::join(&mut l, &mut r.mutty().l);
                    r
                }
            }
        }

        fn split(root: &mut Branch<T>, x: &T) -> (Branch<T>, Branch<T>) {
            if root.is_none() {
                (None, None)
            } else {
                let mut p = mem::take(root);
                if x < &p.reffy().x {
                    let (l, r) = Self::split(&mut p.mutty().l, x);
                    p.mutty().l = r;
                    (l, p)
                } else {
                    let (l, r) = Self::split(&mut p.mutty().r, x);
                    p.mutty().r = l;
                    (p, r)
                }
            }
        }

        pub fn print(&self) -> i32 {
            Self::print_node(&self.root)
        }

        fn print_node(p: &Branch<T>) -> i32 {
            if p.is_none() {
                0
            } else {

                let l = Self::print_node(&p.reffy().l);
                // println!("{}", &p.reffy().x);
                let r = Self::print_node(&p.reffy().r);

                cmp::max(l, r) + 1
            }
        }
    }
}
