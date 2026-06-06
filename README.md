# tedit

<p align="center">
  <img src="assets/tedit_logo.png" alt="tedit logo" width="180">
</p>

<p align="center">A modal terminal text editor built with C++23 and ncurses.</p>

## Overview

`tedit` (text editor) is a small terminal editor with a modal workflow inspired by classic keyboard-driven editors. It currently supports text buffers, a command line, and a basic directory explorer.

## Features

- Modal editing workflow
- Terminal-first interface built on `ncurses`
- Text buffers and a directory explorer
- File-oriented editing with command-line driven actions

## Build

### Requirements

- CMake 3.21 or newer
- A C++23-capable compiler
- `ncurses`

On Debian or Ubuntu:

```bash
sudo apt install build-essential cmake libncurses-dev
```

Build the project:

```bash
git submodule update --init --recursive
cmake -S . -B build
cmake --build build
```

## Run

```bash
./build/tedit
```

By default, the editor opens `assets/file.txt`.

## License

This project is available under the [MIT License](LICENSE).
