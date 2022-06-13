extern "C" {
    fn rand() -> u32;
}

fn get_rand() -> u32 {
    unsafe { rand() }
}

type TreeNode<T> = Option<Box<Node<T>>>;

struct Node<T> {
    value: T,
    height: u32,
    size: usize,
    left: Option<Box<Node<T>>>,
    right: Option<Box<Node<T>>>,
}

impl<T: std::cmp::PartialOrd> Node<T> {
    fn new(x: T) -> Node<T> {
        Node {
            value: x,
            height: get_rand(),
            size: 1usize,
            left: None,
            right: None,
        }
    }

    fn boxed(x: T) -> Box<Node<T>> {
        let node = Node::new(x);
        Box::new(node)
    }

    fn wrap(x: T) -> Option<Box<Node<T>>> {
        Some(Node::boxed(x))
    }

    fn update(&mut self) {
        let left_size = self.left.as_ref().map_or(0, |node| node.size);
        let right_size = self.right.as_ref().map_or(0, |node| node.size);

        self.size = left_size + 1 + right_size;
    }
}

struct Tree<T> {
    root: TreeNode<T>,
}

impl<T: std::cmp::Ord> Tree<T> {
    fn new() -> Tree<T> {
        Tree { root: None }
    }

    fn insert(&mut self, x: T) {
        let (left, right) = Tree::split(self.root.take(), &x);
        self.root = Tree::join(left, Tree::join(Node::wrap(x), right));
    }

    fn join(mut left: TreeNode<T>, mut right: TreeNode<T>) -> TreeNode<T> {
        match (&mut left, &mut right) {
            (None, _) => right,
            (_, None) => left,
            (Some(ref mut lf), Some(ref mut rg)) => {
                if lf.height > rg.height {
                    lf.right = Tree::join(lf.right.take(), right);
                    lf.update();
                    left
                } else {
                    rg.left = Tree::join(left, rg.left.take());
                    rg.update();
                    right
                }
            }
        }
    }

    fn split(mut node: TreeNode<T>, value: &T) -> (TreeNode<T>, TreeNode<T>) {
        match &mut node {
            None => (None, None),
            Some(ref mut p) => {
                if value < &p.value {
                    let (lf, rg) = Tree::split(p.left.take(), value);
                    p.left = rg;
                    p.update();
                    (lf, node)
                } else {
                    let (lf, rg) = Tree::split(p.right.take(), value);
                    p.right = lf;
                    p.update();
                    (node, rg)
                }
            }
        }
    }

    fn size(&self) -> usize {
        return self.root.as_ref().map_or(0, |node| node.size);
    }
}

fn main() {
    let mut t = Tree::new();

    for i in 1..1000_000 {
        t.insert(i);
    }

    println!("{}", t.size());
}
