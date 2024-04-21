[![C/C++ CI](https://github.com/alexlnkp/snekfml/actions/workflows/c-cpp.yml/badge.svg?branch=main)](https://github.com/alexlnkp/snekfml/actions/workflows/c-cpp.yml) <sub>...somehow</sub>

# **snek**-fml

### A quickly hacked together attempt at making lowest size executable possible for a simple snake game on SFML
Decided to make this to introduce myself to SFML. As it turns out - nothing is quite pleasant whenever you try to fight back against the compiler.

## Dependencies
You'll need to install sfml. That's it.

Use your favorite package manager to do so.



Examples on different distros:
- Arch:
    `pacman -Syu sfml`
- Ubuntu:
    `apt-get install libsfml-dev`

On Windows... Uhh... I'm not sure to be entirely honest. You'll probably have to manually link everything... Sorry

## Build
Easy. Use `autobuild` script.
```
Usage: ./autobuild [OPTION] [ARG]

      Common flags:

  -h, --help             Show this help message and exit.
  -d, --debug [X]        Build the program in debug mode. If there's any argument given to this flag - build with GraphicDebugging

      Compression settings:

  -n, --nocompression    [OVERRIDE] - Don't apply any additional compression to compiled executable
  -f, --forcecompression [OVERRIDE] - Force compression on compiled executable

  Note: If no flags are given, the program will be built in release mode with extra compression afterwards.
```
<sub>i.e. just do `./autobuild`.</sub>

## Current state
I will probably forget to update this README afterwards, so here's the state at the moment of creating the README:
- No boundaries! You can fly free straight to the [**65'535** ; **65'535**] coordinates.
- No quick-succesion presses.
- No "grace" period when you're about to hit your own tail.
- No pause button.

# Credit
Font **Angerpoise Lampshade** - TYPODERMIC FONTS INC.