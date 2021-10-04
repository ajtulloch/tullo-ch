---
title: A modern Emacs setup in OS X
date: 16 May 2014
template: article.pug
tags: emacs
---

About a year ago I switched from Vim to Emacs, and I couldn't be
happier about the move.  I spent some time getting a setup I was happy
with, and thought I'd share it for those who are also looking to move
to Emacs.

For more information, my entire `.emacs.d` is available in my
[dots repository][] on GitHub.

## Prerequisites ##

<script
src="https://gist.github.com/ajtulloch/0ba98b93800d9ca97f2a.js"></script>

Install [Pallet][], a wrapper for Cask used to manage your Emacs
packages (think `apt-get`, `yum`, etc.)

Copy [my Cask file][] into your `~/.emacs.d/Cask`, and run `cask
install`. This will pull down all the listed packages into the `.cask`
directory and let you immediately get started.

## Customization ##

In terms of structuring and managing an ever-expanding set of
customizations, I've found the following layout seems to work well.

For your `~/.emacs.d/init.el`, use the following:

<script
src="https://gist.github.com/27e1c1ac1a1e680f5398.js"></script>

This instructs Emacs to initialize packages with Cask/Pallet, and then
loads all the customizations in the order you specify in your
`~/.emacs.d/customizations`.

Entries in the `~/.emacs.d/customizations` are of the form
`02-global.el`.  This guarantees initialization ordering (which
can be useful), and allows you to separate settings naturally (one
file for each language, etc.)

I'll quickly mention a few generic customizations I've added, but see
the full [.emacs.d][dots repository] for a more complete set of
changes (including various customizations for Haskell, Go, Python,
Julia, R, LaTeX, Scala, JS, etc).

### Some Useful Shortcuts ###

This is just a laundry-list of shortcuts I've found useful at various
times.  They rely on various packages and custom functions - see the
full repository for details.

<script src="https://gist.github.com/772afb5cbebf1e8bf2d8.js"></script>

### Magit ###

<script
src="https://gist.github.com/0fcd723f30bd0760ac1d.js"></script>

This makes it cleaner to switch into Magit - simply pressing `C-c C-g`
puts Magit into fullscreen mode, where you can
cleanup/stage/commit/amend/push with only a few presses, and then
pressing `q` takes you back to the window state you had before
entering Magit.  It's really a pleasure to use.

### GitHub Gist utilities ###

<script src="https://gist.github.com/5a631e9dde9d812e3633.js"></script>

This makes it easy to take a buffer or region in Emacs and view it in
various external services - currently [mkdown.com][] for pretty
Markdown viewing and [iPython NBViewer][] for iPython notebooks.
These are the kind of small, useful functions that can be easily
written with elisp (as opposed to the pain of trying to use
vimscript for anything non-trivial).

## Sources of Inspiration ##

A few resources I used over the past year:

- [Yegge's Effective Emacs][] is an excellent resource.
- [Phil Hagelberg's `better-defaults`][] is great (and in the
- aforementioned Cask file).
- [what the emacs.d?][] has some excellent snippets you can use.

[my Cask file]: https://github.com/ajtulloch/dots/blob/cellar-emacs/emacs/Cask
[Pallet]: https://github.com/rdallasgray/pallet
[mkdown.com]: http://mkdown.com
[iPython NBViewer]: http://nbviewer.ipython.org/
[Yegge's Effective Emacs]: https://sites.google.com/site/steveyegge2/effective-emacs
[Phil Hagelberg's `better-defaults`]: https://github.com/technomancy/better-defaults
[what the emacs.d?]: http://whattheemacsd.com/
[dots repository]: https://github.com/ajtulloch/dots/tree/cellar-emacs
