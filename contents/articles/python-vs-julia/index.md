---
title: Python vs Julia - an example from machine learning
date: 11 Mar 2014
template: article.jade
---

In [Speeding up isotonic regression in scikit-learn][previousarticle],
we dropped down into Cython to improve the performance of a regression
algorithm. I thought it would be interesting to compare the
performance of this (optimized) code in Python against the naive Julia
implementation.

This article continues on from [the previous one][previousarticle], so
it may be worth reading that before continuing here to obtain the necessary
background information.

We'll implement both of the algorithms for the previous article, and
compare their performance in Julia against Python.

## Linear PAVA ##

The Cython code is available on GitHub at [`scikit-learn`][sklearn],
and the Julia code is available on GitHub at [Isotonic.jl][isotonicjl]

The Julia implementation is straightforward implementation of PAVA,
without any bells and whistles. The `@inbounds` macro was used to
compare fairly with the Cython implementation, which turns off bounds
checking as well.  

<script src="https://gist.github.com/ajtulloch/9484357.js"></script>
<script src="https://gist.github.com/ajtulloch/9485644.js"></script>

## Active Set ##

The active set implementation is approximately the same number of
lines as the Cython implementation, and is perhaps more cleanly
structured (via an explicit composite type `ActiveState`) that
maintains a given active dual variable's parameters. It is also
trivial to break repeated code into separated functions that
can be trivially inlined by LLVM, while this is difficult for
arbitrary arguments in Cython.

One-based indexing in Julia also made the algorithm somewhat cleaner.

<script src="https://gist.github.com/ajtulloch/9484368.js"></script>
<script src="https://gist.github.com/ajtulloch/9485601.js"></script>

## Performance ##

We see that exactly the same algorithm in Julia is uniformly faster
when compared to an equivalent Cython implementation.

![](https://f.cloud.github.com/assets/1121581/2385599/d0ad76c6-a91f-11e3-84a1-3b2965ba5ea0.png)

For the active set implementations, Julia is anywhere between **5x and
300x faster** on equivalent regression problems.

For the linear PAVA implementation, Julia is between **1.1x and 4x
faster**.

This certainly indicates Julia is a very attractive choice for
performance-critical machine learning applications.

See the [iJulia notebook][] for more information on how these
performance measurements were obtained.

[iJulia notebook]: http://nbviewer.ipython.org/url/gist.githubusercontent.com/ajtulloch/9485996/raw/94b3d0e6bd67256f1f02eebb1463365dbc8b64fc/Julia.ipynb
[previousarticle]: http://tullo.ch/articles/speeding-up-isotonic-regression/
[isotonicjl]: https://github.com/ajtulloch/Isotonic.jl
[sklearn]: https://github.com/scikit-learn/scikit-learn/blob/master/sklearn/_isotonic.pyx
