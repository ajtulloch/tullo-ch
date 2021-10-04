---
title: A Parallel Boggle Solver in Haskell
template: article.pug
date: 5 April 2014
---

A cute interview question I've had is "given an $n \times n$ board of
characters and a dictionary, find all possible words
formed by a self-avoiding path in the grid".  This is otherwise known
as "Boggle".

A simple solution is just conducting a graph traversal of the game board - for
each of the $n^2$ positions, we conduct a DFS search starting at that
position, tracking the previously visited positions at each stage.

This is trivial to do in any language, but I thought it would be
an interesting application of Simon Marlow's `Control.Monad.Parallel`
Haskell library to conduct these traversals  in parallel.

The full implementation is available on GitHub at
<https://github.com/ajtulloch/boggle>.

## Types ##

<script src="https://gist.github.com/ajtulloch/9993421.js"></script>

## Solver ##

<script src="https://gist.github.com/ajtulloch/9993420.js"></script>

## Analysis ##

The key line (indeed, the only line relevant to parallelism) is

```hs
-- construct candidate words in parallel
parallelExpand = concat . parMap rdeepseq expand
```

which (in parallel) expands our current node in the DFS graph, and
concatenates the results together.  Note how trivial it is in a
pure functional language to parallelize a function - compare this to
the equivalent snippet in Python, C++, etc.

Note the type signatures

```
*Main> :t parMap rdeepseq
parMap rdeepseq :: NFData b => (a -> b) -> [a] -> [b]
*Main> :t map
map :: (a -> b) -> [a] -> [b]
```

which indicate that `parMap rdeepseq` is a drop-in replacement for an
existing `map`.

Go is slightly nicer than most, for example:

```go
results := make(chan []int, len(jobs))

for _, job := range jobs {
    go func(job Job) {
        results <- run(job)
    }(job)
}

combined := make([]int, 0)
for _, _ = range jobs {
	for _, p := range <-results {
        combined = append(combined, p)
	}
}
```

although even this pales in comparison to Haskell's elegance here.

## Performance ##

Using GHC runtime system flags, we can control the number of OS
threads used by the runtime, which allows us to control the degree of
parallelism (and hence the amount of speedup from the parallelism)

First, the serial case:

```bash
∴ time dist/build/boggle/boggle 4 +RTS -N1 -H1G -RTS > /dev/null
dist/build/boggle/boggle 4 +RTS -N1 -H1G -RTS > /dev/null  41.12s user
0.50s system 99% cpu 41.652 total
```

```bash
∴ time dist/build/boggle/boggle 4 +RTS -N8 -H1G -RTS > /dev/null
dist/build/boggle/boggle 4 +RTS -N8 -H1G -RTS > /dev/null  63.89s user
1.64s system 744% cpu 8.801 total
```

We see approximately **5x** speedup from executing in parallel across
8 cores (42 seconds to 9 seconds), which indicates our parallelism is
effective here.

## More Information ##

Of course, it's also possible to improve the performance in the
single-threaded case - use a trie for efficiently eliminating large
swathes of the graph by eliminating partial solutions for whom no word
with the given prefix exists in the dictionary, etc.  A hash trie can
determine this in $\mathcal{O}(|q|)$, where $|q|$ is the length of the
prefix up to the given point.

[Discuss this on Hacker News][hn].

[hn]: https://news.ycombinator.com/item?id=7540479
