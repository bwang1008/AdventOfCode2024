#!/usr/bin/bash

set -euo pipefail

# usage:
# $ ./format.sh <file_name>
# Example:
# $ ./format.sh main.cpp

clang-format -i --style=file "$1"
