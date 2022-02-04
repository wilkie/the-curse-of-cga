![](https://github.com/jani-nykanen/the-curse-of-cga/blob/main/dev/release.gif?raw=true)

# The Curse of CGA

...is a puzzle game for MS-DOS with CGA graphics. Or, actually, a prototype of a puzzle game. Or something I call "finished prototype": the game has all the major features, it only lacks of things like audio, options, more animations and a proper story intro (and proper ending). However, I don't want to call it "beta" or "alpha" because there will never be a truly "finished" version, just this "unfinished finished" one. Getting confusing? That's okay.

-----

## Playing

A downloadable binary can be found in the releases: https://github.com/jani-nykanen/the-curse-of-cga/releases. You need DOSBox (https://www.dosbox.com) to play it.

I wanted to have a playable browser build, but em-dosbox and emscripten both refused to work (syntax errors, conflicting files and all that kind of stuff).

### Controls:

Arrow keys for moving, R for restarting, Enter for pausing (you can also find a restart option there)

-----

## Building

You can build `CURSECGA.exe` by placing the `i585-pc-msdosdjgpp-gcc` binary in the `PATH` and invoking `make`:

```
make
```

If you are using a different compiler, or want to give an absolute path to the compiler, you can specify that by using `CC`:

```
make CC=i686-pc-msdosdjgpp-gcc
```

This will create `CURSECGA.exe` in the root path of the project which you can copy along with `ASSETS` to a DOS emulator or real machine!

-----

## License

LGPL, maybe?


-----

(c) 2021 Jani Nykänen

