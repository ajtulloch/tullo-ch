---
title: Modern Emacs setup in OS X
date: 16 May 2014
template: article.jade
tags: emacs
---

About a year ago I switched from Vim to Emacs, and I couldn't be
happier about the move.  I spent some time getting a setup I was happy
with, and thought I'd share it for those who are also looking to move
to Emacs.

## Prerequisites ##

```
$ brew install emacs # Modern Emacs version
$ brew install cask  # Emacs package manager
```

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

[my Cask file]: https://github.com/ajtulloch/dots/blob/cellar-emacs/emacs/Cask
[Pallet]: https://github.com/rdallasgray/pallet
