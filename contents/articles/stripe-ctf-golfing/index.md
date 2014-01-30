---
title: Stripe CTF Distributed Systems - Minimal Solutions
template: article.jade
date: Tue Jan 28 12:12:45 2014
---

The [Stripe CTF][] Distributed Systems edition has just finished, and
I passed all the levels (and was one of the first fifty contestants to
finish). In constructing my solutions, I thought it would be an
interesting challenge to attempt to construct the minimal changes to
the reference solutions that are sufficient to pass the scoring
requirements.

To be clear - these aren't high-quality (or high-scoring) solutions.
I'm not especially proud of these. They are just *small* solutions,
and somewhat interesting for that reason.

[Stripe CTF]: stripe-ctf.com

## Level 0 ##

The challenge is to speed up a simple script that loads a dictionary
into memory, then loops through the input and checks if the input
exists in the dictionary.

<script src="https://gist.github.com/ajtulloch/305491a7edd195ba1358.js"></script>

The trivial solution is just to replace the linear scan in a vector
with a lookup in a hash set (thus replacing an $O(n)$
traversal with $O(1)$ lookup).  This is sufficiently fast to pass.

<script src="https://gist.github.com/ajtulloch/c2ae443eb113cd7089c9.js"></script>

The net diff is two lines changed.

## Level 1 ##

The challenge is to craft a Git commit message with a commit hash less
than (lexicographically) than a given difficutly level.  The reference
implementation is fairly simple, and attached below.

<script src="https://gist.github.com/ajtulloch/3474342810ff771009d0.js"></script>

The bottleneck is the SHA1 computation - we have to compute the hashes
as fast as possible, and the naive solution just doesn't cut it.  To
get around this while reusing the maximum amount of code, we just
replace this inner loop with an 'optimized' (loosely speaking) Python
script that just loops through candidate messages and returns one
satisfying the difficulty level.

Our Python miner is given below

<script src="https://gist.github.com/ajtulloch/22d645c1826a71f6064e.js"></script>

The net difference is six lines changed on the `miner`, and a twenty
line Python helper script.

<script src="https://gist.github.com/ajtulloch/3474342810ff771009d0.js"></script>

## Level 2 ##


The challenge is to rapidly identify elephants from mice - we konw
that elephants send a large number of queries per round, while the
mice send only a few queries per round.

Simply tracking the number of queries per IP (and dropping any queries
past a certain threshold), is sufficient to pass.

<script src="https://gist.github.com/ajtulloch/3dddf877cee3adf0cb50.js"></script>


## Level 3 ##

Level 3 was nice from a creativity standpoint - there were a huge
number of directions we could take for this level.  More efficient indexing,
smarter communication between nodes, better use of memory, etc.

The simplest approach that passed (in terms of number of lines of code
changed) seemed to be was

+ Avoid the overhead of communicating to multiple nodes over HTTP, and
just communicate with a single search node in the same process as the master.
+ Avoid loading files from disk each time, and cache each file in
memory for the duration of the test.

The total lines of code changed were three lines to spin up a
local server instead of remote clients, and two to cache the contents
of each file in memory.  This feels close to optimal for this
problem.

<script src="https://gist.github.com/ajtulloch/e05f75aaa6ba3b5b0241.js"></script>

## Level 4 ##

This was my favourite level in the CTF - Go is like a DSL for
distributed systems, so it was fun to modify.  The challenge was to
make an existing replicated SQL database cluster consistent.  The test
harness spins up five replicas, sends SQL queries to all replicas,
injects failures in the communication between nodes, and tests that
the replication protocol is [sequentially consistent][].

A natural solution is to use the well-tested [`goraft`][goraft]
library for handling distributed replication. The `goraft` API fits
this model naturally - Raft maintains a replicated state machine,
through executing commands from a replicated log.

Thus each SQL command that is executed is an entry in the log, and the
index of this command in the distributed log is the sequence number.
A basic implementation is trivial to implement with `goraft`, see
[`raftd`][raftd] for an example which maintains a distributed KV store
(instead of a distributed SQL database), but is otherwise functionally
identical.

The next issue arises from learning that it is not sufficient to
simply drop queries sent to follower nodes, as doing so results in a
system that does not respond to sufficiently many queries.  There are
a few ways to get around this.  As the test harness does not support
redirects, it appears most chose to proxy requests to leader nodes,
and forward that response from the leader to the test harness.

This approach fails, however, when the following sequence of events
take place:

1. Follower successfully sends request to leader
2. Leader successfully executes command and appends entry to the
replicated log.
3. The link between the follower and leader is disrupted, and thus the
follower assumes the query failed
4. The follower responds to the test harness that the query failed.

In this scenario, there is a discrepancy between what has been
recorded on the distributed log (the query succeed and was committed),
and what was reported to the test harness, and this mean
disqualification.

Thus, we need some kind of way to get around this "false failure". One
simple way is to tag each command with a unique ID before forwarding
to the master, and in the event that a response fails to return to the
master (e.g. transient network failure), we poll the replicated log to
see if our command has been executed by the distributed system. This
has quite ugly properties (we are racing between the test harness
timing out the request to the follower and the replication reaching
the follower), though it works for the case of the harness.

Despite implementing such a powerful distributed consensus algorithm,
the code is fairly short - still the longest problem in this list, but
we can get the implementation done in 162 insertions and 69 deletions.

<script src="https://gist.github.com/ajtulloch/ab5d358a50bbbb569604.js"></script>

[goraft]: https://github.com/goraft/raft
[raftd]: https://github.com/goraft/raftd
[sequentially consistent]: http://en.wikipedia.org/wiki/Sequential_consistency
