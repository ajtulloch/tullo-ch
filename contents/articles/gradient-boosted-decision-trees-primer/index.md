---
template: article.jade
title: A Primer on Gradient Boosted Decision Trees
date: 03/11/2013
---

Gradient boosted decision trees are an effective off-the-shelf method
for generating effective models for classification and regression
tasks.

Gradient boosting is a generic technique that can be applied to
arbitrary 'underlying' weak learners - typically decision trees are
used. The seminal reference is [Friedman's][] [2001 paper][] that
introduced the method.

Consider a typical supervised learning problem - we are given as input
labeled feature vectors $(x, y)$, and seek to estimate a function
$\hat F(x)$ that approximates the 'true' mapping $F^\star$, with
$F^\star$ minimizing the expected loss $L(y, F(x)$ over some candidate
functions $\mathcal{F}$ for a loss function $L$.

<span class="more"/>

In gradient boosting, the model assumes an additive expansion
\begin{equation} F(x, \beta, \alpha) = \sum\_{i=1}^{n} \beta\_{i} h(x,
\alpha\_{i}) \end{equation} where the $h$ are our weak learners. Thus,
the predictor from gradient boosting is a linear combination of weak
learners, and the procedure does two things:

* Computes $\beta_m$ - the weight that a given classifier has in
context of the ensemble.
* Weights the training examples to compute the $i$-th weak classifier
$h(\cdot, \alpha_m)$.

## The Algorithm ##

For examples, we'll use the decision tree training library I wrote in
Go, available on GitHub at https://github.com/ajtulloch/decisiontrees.

The boosting algorithm, in pseudo-code, is quite simple:

* initialize list of weak learners to a singleton list with simple
prior
* for each round in 1..numRounds:
* reweight examples $(x, y)$ to $(x, \tilde y)$ by 'upweighting'
examples that the existing forest poorly predicts
* estimate new weak classifier $h\_i$ on weighted examples
* compute weight $\beta\_i$ of new weak classifier
* add the pair $(h\_i, \beta\_i)$ to the forest
* return forest

The intuition behind gradient boosting is quite simple - we
iteratively build a sequence of predictors, and our final predictor is
a weighted average of these predictors. At each step, we focus on
adding an incremental classifier that improves the performance of the
entire ensemble. The technical description is 'gradient descent in
functional space'.

Thus, if we have examples that are not well predicted by the current
ensemble, the next stage will work harder to fit these examples.

## Example Implementation ##

For an implementation of the above approach, see the [boosting.go][]
file on GitHub. The loop body is a simple function inlined below:

<script src="https://gist.github.com/ajtulloch/7254274.js"></script>

There are some complications (stochastic gradient boosting, influence
trimming), although the core algorithm is as described in the
pseudocode.

In subsequent posts we'll elaborate on

* Algorithms for training the individual weak learners
* Boosting and decision tree hyperparameters
* Speeding up training and prediction

[2001 paper]: http://statweb.stanford.edu/~jhf/ftp/trebst.pdf
[Friedman's]: https://en.wikipedia.org/wiki/Jerome_H._Friedman
[boosting.go]: https://gist.github.com/ajtulloch/7254143
