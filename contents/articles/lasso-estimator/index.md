-------------------------------------------------------------------------------
title: The LASSO Estimator
template: article.jade
date: Sat May 31 2014

-------------------------------------------------------------------------------

As far as I can tell, the LASSO estimator is the closest thing we have
to a miracle in modern statistics.

The LASSO estimator is defined as a solution to the minimization
problem $\frac{1}{n} \\| Y - X \theta \\|_2^2 + \lambda \\| \theta \\|_1$
over $\mathbb{R}^p$. The key insight here is that this is a convex
problem in $\theta$ - this follows from both norms being convex and
the sum of convex functions being convex.  This allows us to design
efficient solvers for this problem and thus handle large-scale
problems - see, for example, ADMM, iterative shrinkage, gradient
projection, etc.

The LASSO can be viewed as convex relaxation of a very natural problem
in statistical estimation - finding the best $k$-sparse vector to
minimize $\\| Y - X \theta \\| + \lambda \\| \theta \\|_0$, where the
$L_0$ norm (indeed, not actually a norm) is to be interpreted as the
number of non-zero coefficients in $\theta$. This comes from problems
such as in signal processing and genomics array data where we have $p$
(the number of covariates) significantly larger than $n$, the number of
observations. In this case, the usual least-squares estimation theory
dating back to Gauss does not apply ($X$ cannot have full rank), and
we must find other alternatives. The brute-force approach is
combinatorially hard (we must check each $p \choose k$ sets of
supports, which takes time exponential in $p$).

Thus, the LASSO objective can be seen as a natural convex relation of
the original problem (e.g. taking $p$ from $0$ upwards and stopping as
soon as $\\| \theta \\|_p$ is convex).

The "miracle" I refer to is the amazing result of Candes & Tao in a
series of papers starting in 2005 that established that for a large
class of observation matrices $X$, we have the amazing result that
with very high probability, solving the LASSO problem is equivalent to
solving the original combinatorially hard problem.  Formally, we have
the following theorem, which contains a germ of the restricted
isometry property.

### The Optimality of the LASSO estimator ###

> Let $\theta\_0$ be $k$-sparse with support $S\_0$, and let $Y = X
> \theta + \epsilon$, with $\epsilon \sim N(0, \sigma^2 I\_n)$. Let
> $\tilde \theta$ be the LASSO estimator of $(Y, X)$ with parameter
> $\lambda = 4 \overline \sigma \sqrt{\frac{t^2 + 2 \log p}{n}}$.
> Assume that $\\| \tilde \theta_{S\_0} - \theta\_0 \\|_1^2 \leq k
> r\_0 (\tilde \theta - \theta\_0)^T \hat \Sigma (\tilde \theta -
> \theta_0)$ with probability at least $1 - \beta$ for some $r_0$.
> Then with probability at least $1 - \beta - e^{-\frac{t^2}{2}}$, we
> have that \begin{equation} \frac{1}{n} \\| X(\tilde \theta -
> \theta_0) \\|_2^2 + \lambda \\| \tilde \theta - \theta_0 \\| \leq 4
> k r_0 \lambda^2 \end{equation}

#### Proof ####

The proof is fairly elementary, requiring only a basic concentration
of measure inequality for subgaussian random variables. The key step
is recognizing that we can bound the event $\max_{j} \frac{2}{n} \\|
(\epsilon^T X)_j \\| \geq \frac{\lambda}{2}$ on a set of measure less
than $e^-\frac{t^2}{2}$. Once we have this result, we can apply the
triangle inequality and the restricted isometry condition in the
theorem to obtain the desired result.



