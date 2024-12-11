# Advent of Code 2024 Solutions

Solutions in C++, compiled using g++ 9.4.0 with CMake 3.16.3 and GNU Make 4.2.1.

Compilation example:

```sh
mkdir build
cd build
cmake -S .. -B .
make
```

Running example:

```sh
./build/solver.out 3 B
```

.clang-tidy generated via

```sh
clang-tidy --dump-config --checks=*,-fuchsia-default-arguments-calls,-fuchsia-trailing-return,-llvm-header-guard -- --std=c++20 > .clang-tidy
```

Ignored checks:

- `fuchsia-default-arguments-calls`: extraneous warnings on standard-library function calls
- `fuchsia-trailing-return`: conflicts with modernize-use-trailing-return-type
- `llvm-header-guard`: do not want to expose full path of file

.clang-format generated via

```sh
clang-format --style=llvm -dump-config > .clang-format
```
