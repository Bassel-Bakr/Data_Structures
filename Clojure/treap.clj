(ns treap
  (:refer-clojure
    :exclude [print remove rand]))

(defn- rand []
  (rand-int Integer/MAX_VALUE))

(defrecord node [x y s l r])

(defn- new-node [x]
  (->node x (rand) 1 nil nil))

(defn size [{s :s}]
  (or s 0))

(defn join
  ([{y1 :y s1 :s r1 :r :as lf}
    {y2 :y l2 :l s2 :s :as rg}]
    (cond
      (nil? lf) rg
      (nil? rg) lf
      (< y1 y2)
        (assoc lf
          :r (join r1 rg)
          :s (inc s1))
      :else
        (assoc rg
          :l (join lf l2)
          :s (inc s2))))
  ([a b & c]
    (reduce join (join a b) c)))

(defn split [{:keys [x l r] :as p} v]
  (cond
    (nil? p)
      [nil nil]
    (< v x)
      (let [[lf rg] (split l v)]
        [lf
         (assoc p
           :l rg
           :s (+ (size rg) 1 (size r)))])
    :else
      (let [[lf rg] (split r v)]
        [(assoc p
           :r lf
           :s (+ (size l) 1 (size lf)))
         rg])))

(defn insert [p v]
  (let [[l r] (split p v)]
    (join l (new-node v) r)))

(defn remove [{:keys [x s l r] :as p} v]
  (cond
    (nil? p) nil
    (< v x)
      (let [l (remove l v)]
        (assoc p
          :l l
          :s (+ (size l) 1 (size r))))
    (> v x)
      (let [r (remove r v)]
        (assoc p
          :r r
      	 	  :s (+ (size l) 1 (size r))))
    :else (join l r)))

(defn remove-range [p l r]
  (let [[a b] (split p r)]
    (let [[x y] (split a (- l 1))]
      (join x b))))

(defn print [{:keys [x l r] :as p}]
  (lazy-seq
    (if (nil? p)
      nil
      (concat (print l)
              (list x)
              (print r)))))

(defn make-treap [a]
  (loop [t nil [f & r] a]
    (if (nil? f)
      t
      (recur (insert t f) r))))
