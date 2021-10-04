---
template: article.pug
date: 26 Nov 2013
title: A basic soft-margin kernel SVM implementation in Python
---

[Support Vector Machines][] (SVMs) are a family of nice supervised learning
algorithms that can train classification and regression models
efficiently and with very good performance in practice.

SVMs are also rooted in convex optimization and Hilbert space theory,
and there is a lot of beautiful mathematics in the derivation of
various aspects of the training algorithm, which we will go into in
subsequent posts.

For now, we'll just give an introduction to the basic theory of
soft-margin kernel SVMs. The classical treatment is to start with
hard-margin linear SVMs, then introduce the kernel trick and the
soft-margin formulation, so this is somewhat faster-moving than other
presentations.

Mathematical Formulation
------------------------

We consider our training set to be

\begin{equation}
D = \{ (\mathbf{x}\_{i}, y\_{i}), \mathbf{x} \in \mathbb{R}^d, y \in \\{ -1, 1 \\}
\}.
\end{equation}

The key idea is that we seek to find a hyperplane $w$ separating
our data - and maximimize the *margin* of this hyperplane to optimize
decision-theoretic metrics.

Let $\kappa$ be a kernel function on $\mathbb{R}^d \times
\mathbb{R}^d$ - a function such that the matrix $K$ with $K\_{ij} =
\kappa(x\_i, x\_j)$ is positive semidefinite.  A key property of such
kernel functions is that there exists a map $\nu$ such that $\langle
\nu(x), \nu(y) \rangle = \kappa(x, y)$.  One can think of $\nu$ as
mapping our input features into a higher dimensional output space.

We can show that for a given feature mapping $\nu$ satisfying the
previous condition, the Lagrangian for the problem of finding the
maximum margin hyperplane takes the form:

\begin{equation}
\inf\_{z \in \mathbb{R}^n} \frac{1}{2} \\left\| \sum\_{i=1}^{n} y\_i \nu(x\_i)
z\_i \\right\|\_2^2 - e^T z
\end{equation}
subject to $z \geq 0$ and $\langle z, y \rangle = 0$.

Given a resulting vector of Lagrange multipliers $z$, we find that
most $z$ are zero. This comes from the complementary slackness
conditions in our optimization problem - either $(x\_i, y\_i)$ is on the
maximum margin (and so corresponding Lagrange multiplier is nonzero),
or it is not on the margin (and so the Lagrange multiplier is zero).

The prediction of a given feature vector $x$ takes the form
\begin{align}
  \label{eq:1}
  \langle w, \nu(x) \rangle &= \sum\_{i=1}^{n} z\_{i} y\_{i} \langle \nu(x\_{i}),
  \nu(x) \rangle \\
  &= \sum\_{i=1}^{n} z\_{i} y\_{i} \kappa(x\_{i}, x)
\end{align} where we can take the sum over only the non-zero $z\_{i}$.

This yields a very efficient prediction algorithm - once we have
trained our SVM, a large amount of the training data (those samples
with zero Lagrangian multipliers) can be removed.

There are more complications (handling the bias term, handling
non-separable datasets), but this is the gist of the algorithm.

Implementation
--------------

The full implementation of the training (using [`cvxopt`][cvxopt] as a
quadratic program solver) in Python is given below:

<script src="https://gist.github.com/ajtulloch/7655363.js"></script>

The code is fairly self-explanatory, and follows the given training
algorithm quite closely.  To compute our Lagrange multipliers, we
simply construct the Gram matrix and solve the given QP.  We then pass
our trained support vectors and their corresponding Lagrange
multipliers and weights to the `SVMPredictor`, whose implementation is
given below.

<script src="https://gist.github.com/ajtulloch/7655399.js"></script>

This simply implements the above prediction equation.

A sample list of kernel functions are given in

<script src="https://gist.github.com/ajtulloch/7655415.js"></script>

Demonstration
-------------

We demonstrate drawing pairs of independent standard normal variables
as features, and label $y\_i = sign(\sum x)$.  This is trivially
linearly seperable, so we train a linear SVM (where $\kappa(x\_i, x\_j) =
\langle x\_i, x\_j \rangle$) on the sample data.

We then visualize the samples and decision boundary of the SVM on this
dataset, using [`matplotlib`][matplotlib]. See [this gist][] for
details on the implementation.

An example output of this demonstration is given below:

![SVM Demonstration](http://i.imgur.com/yy0oUVk.png)

More Information
----------------

See the [`svmpy`][svmpy] library on GitHub for all code used in this post.

[svmpy]: https://github.com/ajtulloch/svmpy
[this gist]: https://gist.github.com/ajtulloch/7655467
[matplotlib]: http://matplotlib.org/
[cvxopt]: http://cvxopt.org/
[Support Vector Machines]: http://en.wikipedia.org/wiki/Support_vector_machine
