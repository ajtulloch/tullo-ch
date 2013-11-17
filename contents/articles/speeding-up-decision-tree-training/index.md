---
template: article.jade
title: Speeding up Decision Tree Training
date: 03 Nov 2013
---

The classic algorithm for training a decision tree for
classification/regression problems (CART) is well known. The
underlying algorithm acts by recursively partitioning the dataset into
subsets that maximize the 'clustering' of examples in each of the
partitioned subsets, where the metric used for clustering varies
depending on the problem (for example, information gain, Gini loss,
etc, have been used successfully in the literature).

For a high level overview of the algorithm, see the following snippet
of [Haskell code][hc] code from the [haskell-ml project][hml] project.

<span class="more"/>

<script src="https://gist.github.com/ajtulloch/7295132.js"></script>

This is the simplest possible implementation - for each possible
feature, we sort the (label, feature) pairs and compute the optimal
splitting point for each feature, according to our decision metric. We
then take the 'best' possible split, split the examples by that point,
record that we split the current node at the given feature and value,
and recur down the left and right sides.

The inner loop of the algorithm (in Python) is as follows (function
`get_best_split`):

<script src="https://gist.github.com/ajtulloch/7295006.js"></script>

The complexity of this naive implementation is $\mathcal{O}(F \cdot
E^3 \log E)$ - where $F$ are the number of features and $E$ is the
number of examples. This is because we loop over the features $(|F|)$,
sort the examples ($\mathcal{O}(E \log E)$) then over each example
($|E|$), and computing the loss from partitioning at the given example
takes $\mathcal{O}(E)$ time.

It is important to note that this can be incredibly slow (consider
when we have $\mathcal{O}(10^4)$ features and $\mathcal{O}(10^{10})$
examples). There are several well-known ways we can speed this up.

## Speeding up decision tree training ##

There are several ways we can make this process faster.

* Incrementally updating the gain at a given split instead of
recomputing the update.
* Parallelizing recursive tree construction steps.
* For gradient boosting, we can trim low-importance samples
(*influence trimming*), or just consider only a subset (*stochastic
gradient boosting*).
* Considering only a random subset of features and examples at each
iteration - as in *random forests*.

We'll go through these in turn, with code examples from the
[decisiontrees library][] on GitHub - a backend and frontend for
training gradient boosted decision trees, random forests, etc. written
in Go. In particular, the [`regression_splitter.go`][rsg],
[`random_forest.go`][rfg], and [`boosting.go`][bg] files are where a
lot of these techniques are implemented.

### Incrementally computing the loss ###

A simple optimization can take the computation of the loss at any
given point from $\mathcal{O}(E)$ to $\mathcal{O}(1)$ for a large set
of loss functions. Consider the case where we minimize $L^2$ loss on
the splits. Thus, the loss on a given subset is
\begin{equation}
L(S) = \sum\_{s \in S} (s - \overline S)^{2}
\end{equation}

By using the online update formula for the variance of a set of
samples - which for a stream of samples $x\_{1}, \dots, x\_{n}$,
allows us to compute the variance of $\mathbb{V}(x\_{1}, \dots,
x\_{n+1}) = \mathbb{V}\_{n+1}$ given $\mathbb{V}\_{n}$ and the value
$x\_{n+1}$ in constant time and space by tracking $\sum\_{i=1}^{n}
x\_{i}^{2}$ and $\sum\_{i=1}^{n} x\_{i}$.

See below for the implementation of this approach for $L^2$ loss.

<script src="https://gist.github.com/ajtulloch/7293348.js"></script>

### Parallelizing recursive tree construction steps ###

Note that once we have decided to split at a given node, there is no
data sharing between the procedures that compute the left side of the
tree and the right side of the tree. Thus, we can compute these in
parallel, and can speed up computation significantly on systems with
multiple CPUs - asymptotically up to $B$ times faster where $B$ is the
branching factor on our branch.

See below for an implementation of this approach.

<script src="https://gist.github.com/ajtulloch/7293383.js"></script>

### Parallelizing finding the optimal split ###

The key insight here is that finding the best split amongst $|F|$
features can be done by forking $|F|$ processes to search through each
features possible splits in parallel, then joining and finding the
best candidate split from each subroutine.

The tradeoff in this approach is that $|F|$ copies of the examples
must be passed to each subroutine - as the subroutines sort these
examples which requires ownership of a copy of the data. If we just
pass a cheap copy of pointers to the examples (e.g.
`std::vector<Example*>` in `C++`), we can easily reduce this cost.
This speedup depends on the relative sizes of $|F|$ and $|E|$ and the
cost of memory allocation in the given system, but is in general a
significant speedup.

See below for an example implementation in Go, using channels to
communicate splits back to the master thread.

<script src="https://gist.github.com/ajtulloch/7293393.js"></script>

### Influence Trimming and Stochastic Gradient Boosting ###

In gradient boosting (and boosting algorithms in general), we weight
examples by their degree of misclassification by the ensemble thus
far. The intuition is that each incremental stage is "trained on the
residuals" of the previous stage.

At each stage, we compute a weight metric for each example $w\_i$,
representing the influence of a given sample of the next stage. In
practice, the distribution of influence over examples follows a power
law, so trimming the bottom $l\_\alpha$ samples, where
\begin{equation} \sum\_{i=1}^{l\_{\alpha}} w\_{i} = \alpha
\sum\_{i=1}^{N} w\_{i} \end{equation} for $\alpha$ between 5% and 20%
can remove a large fraction of samples

In the [paper][] introducing gradient boosting, Friedman notes that up
to 90%-95% of examples at later stages can be reduced without a
measurable loss in accuracy.

<script src="https://gist.github.com/ajtulloch/7292976.js"></script>

In a [follow up paper][] to the initial gradient boosting machine
paper, Friedman introduces stochastic gradient boosting - at each
iteration, select a random subset of examples for the construction of
the next weak learner. Friedman's experiments indicated that 20%-50%
of examples can be dropped at any given stage without a significant
loss in the quality of the ensemble. Given the dependence on the
number of examples on the time spent training, this can be a useful
improvement.

See the following code for the implementation of a boosting round:

<script src="https://gist.github.com/ajtulloch/7292981.js"></script>

### Random Forests ###

When using [Brieman's algorithm][] to train random forests, there are
several key speedups over naive ensemble construction:

* For each weak classifier, choose the best splits with $m \ll |F|$
features, and a boostrap sample of size $n < |E|$ examples.
* Each weak classifier is trained independently of the others (as
opposed to gradient boosting), and so can be trivially parallelized.

For example, see the following code that uses Golang's convenient
[`sync.WaitGroup`][swg] abstraction for computing the weak learners in
parallel.

<script src="https://gist.github.com/ajtulloch/7292836.js"></script>

## Conclusion ##

We've talked about a number of methods that can be used for evaluation
time improvement. Please have a look at the [decisiontrees library][]
for an integrated view of how these are implemented. In subsequent
posts, we'll talk about the other side of the equation - speeding up
evaluation of decision trees.

[hc]: https://github.com/ajtulloch/haskell-ml/blob/master/MachineLearning/DecisionTrees.hs
[hml]: https://github.com/ajtulloch/haskell-ml/
[follow up paper]: http://statweb.stanford.edu/~jhf/ftp/stobst.pdf
[paper]: http://statweb.stanford.edu/~jhf/ftp/trebst.pdf
[swg]: http://golang.org/pkg/sync/#WaitGroup 
[Brieman's algorithm]: https://en.wikipedia.org/wiki/Random_forest#Breiman.27s_Algorithm
[decisiontrees library]: https://github.com/ajtulloch/decisiontrees
[rsg]: https://github.com/ajtulloch/decisiontrees/blob/master/regression_splitter.go
[rfg]: https://github.com/ajtulloch/decisiontrees/blob/master/random_forest.go
[bg]: https://github.com/ajtulloch/decisiontrees/blob/master/boosting.go
