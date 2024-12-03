#!/usr/bin/bash

set -euo pipefail

# usage:
# $ ./lint.sh <file_name>
# Example:
# $ ./lint.sh main.hpp

clang-tidy "$1" -config='' -- -std=c++20
