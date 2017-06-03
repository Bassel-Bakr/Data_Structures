class Treap {
    data class Node(var x: Int,
                    var l: Node? = null, var r: Node? = null,
                    var s: Int = 1, val y: Int = (Math.random() * 100_000).toInt())
    
    fun Node.upd(): Node {
        s = 1
        s += l?.s ?: 0
        s += r?.s ?: 0
        return this
	}
    
    var root: Node? = null
    
    fun join(l: Node?, r: Node?) : Node? {
        if(l == null) return r
        if(r == null) return l
        
        if(l.y < r.y) {
        	l.r = join(l.r, r)
            return l.upd()
        } else {
        	r.l = join(l, r.l)
            return r.upd()
		}
	}
    
    fun split(p: Node?, x: Int): Pair<Node?, Node?> {
        if(p == null)
        	return Pair(null, null)
        
        if(x < p.x) {
        	var (l, r) = split(p.l, x)
            p.l = r
            r = p.upd()
            return Pair(l, r)
        } else {
        	var (l, r) = split(p.r, x)
            p.r = l
            l = p.upd()
            return Pair(l, r)
		}
	}
    
    fun insert(x: Int) {
        val (l, r) = split(root, x)
        root = join(l, join(Node(x), r))
	}
    
    fun remove(x: Int) {
        root = remove(root, x)
	}
    
    fun print(): Int {
        return print(root)
	}
    
    fun remove(p: Node?, x: Int): Node? {
        if(p == null)
        	return null
        
        if(x == p.x)
        	return join(p.l, p.r)
        
        if(x < p.x)
        	p.l = remove(p.l, x)
        else
        	p.r = remove(p.r, x)
        
        return p.upd()
	}
    
    fun print(p: Node?): Int {
        if(p == null)
        	return 0
        
        val l = print(p.l)
        println(p.x)        
        val r = print(p.r)
        
        return 1 + intArrayOf(l, r).max()!!
    }
}

fun main(args: Array<String>) {
    val n = 100
    
    val t = Treap()
    for(i in 0 until n)
    	t.insert(i)
    
    t.print()
}
