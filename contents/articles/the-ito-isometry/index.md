---
template: article.jade
title: The Ito isometry
date: 03 Nov 2013
tags: statistics, mathematics
---

The *Itō isometry* is a useful theorem in stochastic calculus that
provides a fundamental tool in computing stochastic integrals -
integrals with respect to a Brownian motion \begin{equation}
\int\_{0}^{\infty} f(s) dB\_{s} \end{equation} with $B\_{s}$ a
Brownian motion.

First, we'll define a predictable process.

<span class="more"></span>

### Definition ###

A stochastic process $\alpha\_{t}$ is called a *simple predictable
process* if it is of the form \begin{equation} \alpha\_{t} =
\sum\_{i=1}^{n} \mathbb{I}\_{(t \_{i-1}, t\_{i}]} a\_{i}
\end{equation} for $0 \leq t\_{0} < \dots < t\_{n}$, with $a\_{i}$ a
bounded $\mathcal{F}\_{t\_{i-1}}$-measurable random variable.

This definition is useful as we can construct sequences of simple
predictable processes that converge in $L^{2}$ to our stochastic
processes of interest (under certain technical conditions).

The *Itō isometry* is the following result

### Theorem ###

Let $\alpha\_{t}$ be a simple predictable process. Then

\begin{equation} \mathbb{E} \left(\int\_{0}^{\infty} \alpha\_{s}
  dB\_{s} \right)^{2} = \mathbb{E} \int\_{0}^{\infty} \alpha\_{s}^{2}
  ds \end{equation}

Thus, the mapping from simple predictable process to square integrable
random variables on $L^{2}(\Omega, \mathcal{F}, \mathbb{P})$ (which is
complete) defined by \begin{equation} I(\alpha) = \int\_{0}^{\infty}
\alpha\_{s} dW\_{s} \end{equation} is an isometry.

### Proof ###

The proof is a fairly straightforward application of the properties of
the Brownian motion.

The expectation of the square of $\alpha\_{t}$ becomes a sum of
expectations on disjoint subsets of
$[0, \infty)$ (which are zero by the independence of Brownian motion in two intervals), and the sum of the expectation of $a\_{i}^{2} (dB\_{s})^{2}$ on the interval $(t\_{i-1}, t\_{i}]$,
which is $a\_{i}^{2} (t\_{i} - t\_{i-1})$ as the increments of the
Brownian motion are normally distributed with mean zero and variance
equal to the size of the interval.
