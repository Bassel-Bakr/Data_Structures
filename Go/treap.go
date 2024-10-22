package main

import (
	"cmp"
	"math/rand/v2"
)

type Node[T cmp.Ordered] struct {
	Value       T
	heapValue   int
	Left, Right *Node[T]
}

func NewNode[T cmp.Ordered](x T) *Node[T] {
	return &Node[T]{
		Value:     x,
		heapValue: rand.Int(),
	}
}

func (left *Node[T]) join(right *Node[T]) *Node[T] {
	if left == nil {
		return right
	} else if right == nil {
		return left
	} else if left.heapValue < right.heapValue {
		left.Right = left.Right.join(right)
		return left
	} else {
		right.Left = left.join(right.Left)
		return right
	}
}

func (node *Node[T]) split(value T) (*Node[T], *Node[T]) {
	if node == nil {
		return nil, nil
	} else if cmp.Compare(value, node.Value) < 0 {
		leftLeft, leftRight := node.Left.split(value)
		node.Left = leftRight
		return leftLeft, node
	} else {
		rightLeft, rightRight := node.Right.split(value)
		node.Right = rightLeft
		return node, rightRight
	}
}

func (node *Node[T]) insert(value T) *Node[T] {
	left, right := node.split(value)
	return left.join(NewNode(value)).join(right)
}

func (node *Node[T]) print() {
	if node == nil {
		return
	}
	node.Left.print()
	println(node.Value)
	node.Right.print()
}

func main() {

	var t *Node[int]

	for i := range 100_000 {
		t = t.insert(i)
	}

	t.print()
}
