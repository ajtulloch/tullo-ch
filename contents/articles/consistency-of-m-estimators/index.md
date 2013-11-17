---
title: Consistency of M-estimators
date: 03 Nov 2013
template: article.jade
tags: statistics
---

Let $\Theta \subseteq \mathbb{R}^{p}$ be compact. Let $Q: \Theta \rightarrow
\mathbb{R}$ be a continuous, non-random function that has a unique minimizer
$\theta\_{0} \in \Theta$.

Let $Q\_{n}: \Theta \rightarrow \mathbb{R}$ be any sequence of random
functions such that

\begin{equation} 
  \sup\_{\theta \in \Theta} |Q\_{n}(\theta) - Q(\theta)| \rightarrow 0 
\end{equation}
as $n \rightarrow \infty$.

If $\theta\_{n}$ is *any* sequence of minimizers of $Q\_{n}$,
then $\hat \theta\_{n} \rightarrow \theta\_{0}$ in probability as $n \rightarrow \infty$.
