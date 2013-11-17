---
title: Purely Functional Tree Search in Haskell
date: 1 Nov 2013
template: article.jade
---

Haskell is an absolute pleasure to write code in, and I've been trying to use it more and more. It's a language that rewards extended effort in a way that `C++` et. al. do not.

Consider the following program, illustrating a basic BFS/DFS search through a tree in Haskell.  It illustrates a number of useful concepts - [algebraic data types][], [type classes][], and [QuickCheck][].

<span class="more"></span>

The code is straightforward (as Haskell code tends to be) - we define our basic tree structure, describe some type classes it belongs to, and traverse the tree.

e<script src="https://gist.github.com/ajtulloch/7236639.js"></script>

## QuickCheck ##

Assume there was a bug in our code - for example, we replace an `||` with an `&&` in our `search` function, going from

<script src="https://gist.github.com/ajtulloch/7236642.js"></script>

to 

<script src="https://gist.github.com/ajtulloch/7236654.js"></script>

Running this incorrect program, QuickCheck immediately identifies the minimal example for which this program fails.

<script src="https://gist.github.com/ajtulloch/7236669.js"></script>

See the [chapter][] on QuickCheck and other forms of testing in [Real World Haskell][] for more information.

[Real World Haskell]: http://book.realworldhaskell.org/
[chapter]: http://book.realworldhaskell.org/read/testing-and-quality-assurance.html
[QuickCheck]: http://en.wikipedia.org/wiki/QuickCheck
[algebraic data types]: http://www.haskell.org/haskellwiki/Algebraic_data_type
[type classes]: https://en.wikipedia.org/wiki/Type_class
