# Advent of Code 2024 Solutions

Solutions in C++, compiled using g++ 9.4.0.

Compilation example:

```bash
g++ -std=c++2a -Wall -Wextra -Wpedantic -pedantic-errors -Wmissing-braces -Wmissing-include-dirs -Wswitch-default -Wunused -Wuninitialized -Wunknown-pragmas -Wmissing-noreturn -Wsuggest-attribute=pure -Wsuggest-attribute=const -Wsuggest-attribute=noreturn -Wsuggest-attribute=malloc -Wbool-compare -Wduplicated-branches -Wduplicated-cond -Wdouble-promotion -Wfloat-equal -Wshadow -Wunsafe-loop-optimizations -Wunused-macros -Wcast-qual -Wconversion -Wdate-time -Wsign-conversion -Wlogical-op -Wredundant-decls -fno-operator-names -fno-common -fsanitize=address -fstack-protector-all AdventOfCode2024/main.cpp AdventOfCode2024/day01.cpp AdventOfCode2024/day02.cpp AdventOfCode2024/day03.cpp -o a.out
```

Running example:

```bash
./a.out 3 B
```

.clang-tidy generated via

```bash
clang-tidy --dump-config --checks=*,-fuchsia-default-arguments-calls,-fuchsia-trailing-return,-llvm-header-guard -- --std=c++20 > .clang-tidy
```

Ignored checks:

- fuchsia-default-arguments-calls: extraneous warnings on standard-library function calls
- fuchsia-trailing-return: conflicts with modernize-use-trailing-return-type
- llvm-header-guard: do not want to expose full path of file

.clang-format generated via

```bash
clang-format --style=llvm -dump-config > .clang-format
```
