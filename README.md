Simple shell implementation on C using fork and signals.

## Features:
1. Background tasks running using `&` in the end of command.
2. `cd` support for switching directory.
3. `bg` builtin function for showing all background tasks.
4. `term` builtin function for killing one of background tasks by index.

## Quick start:
```bash
git clone https://github.com/DieTime/SimpleShell.git
cd SimpleShell/

cmake -S . -B ./cmake-build 
cmake --build ./cmake-build

cd ./cmake-build && ./shell
help
```
