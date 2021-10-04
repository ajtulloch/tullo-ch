---
template: article.pug
title: Speeding up isotonic regression in scikit-learn by 5,000x
date: 9 Mar 2014
---

[Isotonic regression][] is a useful non-parametric regression
technique for fitting an increasing function to a given dataset.

A classic use is in improving the calibration of a probabilistic
classifier.  Say we have a set of 0/1 data-points (e.g. ad clicks), and
we train a probabilistic classifier on this dataset.

Unfortunately, we find that our classifier is poorly calibrated - for
cases where it predicts ~50% probability of a click, there is actually
a 20% probability of a click, and so on.

In this case, we can learn an isotonic regression model on the output
of the classifier, where our increasing function we fit is $\mathcal{P}(+ \, | \,
\text{classifiers prediction})$.  The constraint that the function is
increasing means that the ordering of events is preserved by the
transformation, which is an important constraint.

With a trained isotonic regression model, our final output is the
composition of the classifiers prediction with the isotonic regression
function.

For an example of this usage, see the Google
[Ad Click Prediction - A View from the Trenches][adclickprediction]
paper from KDD 2013, which covers this technique in section 7. The
[AdPredictor ICML paper][adpredictor] paper also uses this technique
for calibrating a Naive Bayes predictor.

We'll now detail how we made the [`scikit-learn`][scikit-learn]
implementation of isotonic regression more than ~5,000x faster, while
reducing the number of lines of code in the implementation.

## The Pooled Adjacent Violators Algorithm ##

The pooled adjacent violators algorithm (PAVA) is a useful algorithm
for fitting weighted isotonic regressions to data.

PAVA is a linear-time algorithm for fitting an isotonic regression
model. There is a nice visualization and explanation at
[Fabian Pedregosa's blog][fpblog].

> The algorithm sweeps through the data looking for violations of the
> monotonicity constraint. When it finds one, it adjusts the estimate to
> the best possible fit with constraints. Sometimes it also needs to
> modify previous points to make sure the new estimate does not violate
> the constraints.

See [Isotone Optimization in R][] for a more
formal introduction to PAVA and active set methods for isotone regression.

[Isotone Optimization in R]: http://www.jstatsoft.org/v32/i05/paper

## Speeding up Isotonic Regression ##

I wrote the current scikit-learn implementation after seeing the
previous implementation was much slower than equivalent
implementations in R, etc.

The original algorithm used the active set, which is in some sense
mathematically dual to PAVA, and seemed to scale quite slowly
(approximately $\mathcal{O}(N^2)$). This is despite being implemented
in Cython, which compiles Python-like code to C, and is generally very
fast. After some profiling, it turned out this slowness was due to the
$\mathcal{O}(N)$ list pop in the inner loop (lines 29 and 39 of the
below gist)

<script src="https://gist.github.com/ajtulloch/9447845.js"></script>

Additionally, the implementation was not amenable to efficient machine
code generation, given the large number of calls to the native Python
in the generated Cython code.  In the below image, white lines are
essentially translated to C, while yellow lines call into the Python
API, and are thus much more expensive.

![](https://f.cloud.github.com/assets/1121581/2368135/c715541e-a797-11e3-818e-286982071d96.png)

Additionally, the algorithm allocates a significant amount of memory,
due to maintaining and updating the auxiliary data structure
`active_sets`.

Thus, it seemed promising to seek to improve the performance of this
code. Profiling (using `Instruments.app` on OS X) indicated that the
heavy memory allocation and the list `pop` were the key causes of
slowdown, so an efficient Cython implementation of PAVA seemed a
natural choice.  PAVA can be implemented entirely in-place (so no
additionally memory allocations are needed) and does not require any
operations beyond indexing into arrays, so can be efficiently compiled
by Cython.

Once a simple benchmarking script had been written (to quickly
validate performance improvements), we set about implementing PAVA.
The PAVA implementation is gisted below.

<script src="https://gist.github.com/ajtulloch/9447957.js"></script>

A few interesting aspects of this approach:

1. No memory allocations are made - mutations are made in-place to the
`solution` vector.
2. The algorithm is essentially pure C with a Python syntax - the type
information available to the compiler means that the generated code is
as efficient as a pure C implementation.
3. This algorithm is essential $\mathcal{O}(N^2)$ - we don't use the
optimization of collapsing subsequences into a single point, which is
required to achieve $\mathcal{O}(N)$ asymptotics.  Experiments
indicated this optimization didn't improve performance.

The output of `cython --annotate` indicates that we have effectively
eliminated all Python API calls, and are being translated into
straightforward C.

![](https://f.cloud.github.com/assets/1121581/2368157/97ba2770-a798-11e3-8bbb-9693a0f62ffa.png)

Performance
-----------

Benchmark results indicate the simple PAVA algorithm performs much
faster - **approximately 5,000x faster with 1,000,000 data-points,
approximately 500x faster with 100,000 data-points, and 14x faster with
1,000 data-points**.

On a log-log scale, the performance improvements are visualized below
for two separate datasets - a randomly perturbed version of $\log(1 +
x)$, and a simulated univariate logistic regression dataset.

![](https://f.cloud.github.com/assets/1121581/2368112/bb6135a8-a796-11e3-8168-727abed7ec24.png)
![](https://f.cloud.github.com/assets/1121581/2368113/bb7482f2-a796-11e3-8b3d-aeafa802a717.png)

## The Importance of Being Typed ##

It is also instructive to examine the effect that Cython type
information has on the performance of the PAVA implementation.
Stripping the `cdef` block yields the following comparative
performance:

![](https://f.cloud.github.com/assets/1121581/2368114/bb76e7f4-a796-11e3-8adf-ed59295b4026.png)

Thus, stripping out the type annotations can makes the same Cython algorithm
**~100x-500x slower**.

There's a lesson here in the importance of constant factors when doing
performance-critical work - and another one on understanding all
stages of your tool-chain.

A ~100x-500x performance hit from not adding type information is
incredibly high, and is both a reflection of the speed of
straight-line C code, and the cost of falling back to the Python interpreter.

## More Information ##

See the [GitHub pull request][] for more information and discussion.

[GitHub pull request]: https://github.com/scikit-learn/scikit-learn/pull/2944
[fpblog]: http://fa.bianp.net/blog/2013/isotonic-regression/
[adclickprediction]: http://static.googleusercontent.com/media/research.google.com/en//pubs/archive/41159.pdf
[Isotonic regression]: http://en.wikipedia.org/wiki/Isotonic_regression
[adpredictor]: http://research.microsoft.com/pubs/122779/AdPredictor%20ICML%202010%20-%20final.pdf
[scikit-learn]: http://scikit-learn.org/
