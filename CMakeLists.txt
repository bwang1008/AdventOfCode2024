CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

PROJECT(AdventOfCode2024
	VERSION 0.0
	DESCRIPTION "C++ solutions to Advent of Code 2024"
	LANGUAGES CXX
)

####################
### Main targets ###
####################

ADD_EXECUTABLE(
	solver.out
	AdventOfCode2024/day.hpp
	AdventOfCode2024/main.cpp
)

ADD_LIBRARY(
	utils SHARED
	AdventOfCode2024/utils.cpp
	AdventOfCode2024/utils.hpp
)

ADD_LIBRARY(
	day01 SHARED
	AdventOfCode2024/day.hpp
	AdventOfCode2024/day01.cpp
)
TARGET_LINK_LIBRARIES(day01 utils)

ADD_LIBRARY(
	day02 SHARED
	AdventOfCode2024/day.hpp
	AdventOfCode2024/day02.cpp
)
TARGET_LINK_LIBRARIES(day02 utils)

ADD_LIBRARY(
	day03 SHARED
	AdventOfCode2024/day.hpp
	AdventOfCode2024/day03.cpp
)
TARGET_LINK_LIBRARIES(day03 utils)

ADD_LIBRARY(
	day04 SHARED
	AdventOfCode2024/day.hpp
	AdventOfCode2024/day04.cpp
)
TARGET_LINK_LIBRARIES(day04 utils)

ADD_LIBRARY(
	day05 SHARED
	AdventOfCode2024/day.hpp
	AdventOfCode2024/day05.cpp
)
TARGET_LINK_LIBRARIES(day05 utils)

ADD_LIBRARY(
	day06 SHARED
	AdventOfCode2024/day.hpp
	AdventOfCode2024/day06.cpp
)
TARGET_LINK_LIBRARIES(day06 utils)

ADD_LIBRARY(
	day07 SHARED
	AdventOfCode2024/day.hpp
	AdventOfCode2024/day07.cpp
)
TARGET_LINK_LIBRARIES(day07 utils)

ADD_LIBRARY(
	day08 SHARED
	AdventOfCode2024/day.hpp
	AdventOfCode2024/day08.cpp
)
TARGET_LINK_LIBRARIES(day08 utils)

ADD_LIBRARY(
	day09 SHARED
	AdventOfCode2024/day.hpp
	AdventOfCode2024/day09.cpp
)
TARGET_LINK_LIBRARIES(day09 utils)

ADD_LIBRARY(
	day10 SHARED
	AdventOfCode2024/day.hpp
	AdventOfCode2024/day10.cpp
)
TARGET_LINK_LIBRARIES(day10 utils)

ADD_LIBRARY(
	day11 SHARED
	AdventOfCode2024/day.hpp
	AdventOfCode2024/day11.cpp
)
TARGET_LINK_LIBRARIES(day11 utils)

ADD_LIBRARY(
	day12 SHARED
	AdventOfCode2024/day.hpp
	AdventOfCode2024/day12.cpp
)
TARGET_LINK_LIBRARIES(day12 utils)

ADD_LIBRARY(
	day13 SHARED
	AdventOfCode2024/day.hpp
	AdventOfCode2024/day13.cpp
)
TARGET_LINK_LIBRARIES(day13 utils)

ADD_LIBRARY(
	day14 SHARED
	AdventOfCode2024/day.hpp
	AdventOfCode2024/day14.cpp
)
TARGET_LINK_LIBRARIES(day14 utils)

TARGET_LINK_LIBRARIES(
	solver.out
	day01
	day02
	day03
	day04
	day05
	day06
	day07
	day08
	day09
	day10
	day11
	day12
	day13
	day14
)

##############################
### Compile and link flags ###
##############################

SET(WARNINGS
	-Wall                         # enable many other warnings
	-Wextra                       # more warnings
	-Wpedantic                    # strict ISO C++
	-pedantic-errors              # depends on c++ standard used
	-Wmissing-braces              # initalizers have proper braces
	-Wmissing-include-dirs        # if include directory not valid
	-Wswitch-default              # case statements should have default case
	-Wunused                      # all -Wunused-* warnings
	-Wuninitialized               # use without initialization
	-Wunknown-pragmas             # do not use unknown   #pragma
	-Wmissing-noreturn            # if we should have attribute [[noreturn]]
	-Wsuggest-attribute=pure      # warn if some functions should have these attributes
	-Wsuggest-attribute=const
	-Wsuggest-attribute=noreturn
	-Wsuggest-attribute=malloc
	-Wbool-compare                # prevent comparing boolean with int
	-Wduplicated-branches         # prevent branch statements that provide same value
	-Wduplicated-cond             # prevent same condition in if and else if
	-Wdouble-promotion            # warn if float implicitly casts to double
	-Wfloat-equal                 # do not use equality on floating-points
	-Wshadow                      # prevent shadowing of local variables
	-Wunsafe-loop-optimizations   # warn if compiler cannot optimize loop
	-Wunused-macros               # warn about macros that are not expanded anywhere
	-Wcast-qual                   # warn if casting removes type-qualifiers
	-Wconversion                  # warn about conversions that may change the value
	-Wdate-time                   # do not use __TIME__, __DATE__, or __TIMESTAMP__ to get reproducible builds
	-Wsign-conversion             # warn about conversions between signed and unsigned integers
	-Wlogical-op                  # warn about suspicious usages of logical operations ||, &&, ^
	-Wredundant-decls             # warn when multiple declarations
	-fno-operator-names           # do not use "and", "or", and "xor" (and others) as keywords
	-fno-common                   # prevent tentative definitions
	-fsanitize=address            # detect memory access errors
	-fstack-protector-all         # stack protection
)


TARGET_COMPILE_FEATURES(utils PUBLIC cxx_std_20)
SET_TARGET_PROPERTIES(utils PROPERTIES CXX_EXTENSIONS OFF)
TARGET_COMPILE_OPTIONS(utils PRIVATE "${WARNINGS}")

TARGET_COMPILE_FEATURES(day01 PUBLIC cxx_std_20)
SET_TARGET_PROPERTIES(day01 PROPERTIES CXX_EXTENSIONS OFF)
TARGET_COMPILE_OPTIONS(day01 PRIVATE "${WARNINGS}")

TARGET_COMPILE_FEATURES(day02 PUBLIC cxx_std_20)
SET_TARGET_PROPERTIES(day02 PROPERTIES CXX_EXTENSIONS OFF)
TARGET_COMPILE_OPTIONS(day02 PRIVATE "${WARNINGS}")

TARGET_COMPILE_FEATURES(day03 PUBLIC cxx_std_20)
SET_TARGET_PROPERTIES(day03 PROPERTIES CXX_EXTENSIONS OFF)
TARGET_COMPILE_OPTIONS(day03 PRIVATE "${WARNINGS}")

TARGET_COMPILE_FEATURES(day04 PUBLIC cxx_std_20)
SET_TARGET_PROPERTIES(day04 PROPERTIES CXX_EXTENSIONS OFF)
TARGET_COMPILE_OPTIONS(day04 PRIVATE "${WARNINGS}")

TARGET_COMPILE_FEATURES(day05 PUBLIC cxx_std_20)
SET_TARGET_PROPERTIES(day05 PROPERTIES CXX_EXTENSIONS OFF)
TARGET_COMPILE_OPTIONS(day05 PRIVATE "${WARNINGS}")

TARGET_COMPILE_FEATURES(day06 PUBLIC cxx_std_20)
SET_TARGET_PROPERTIES(day06 PROPERTIES CXX_EXTENSIONS OFF)
TARGET_COMPILE_OPTIONS(day06 PRIVATE "${WARNINGS}")

TARGET_COMPILE_FEATURES(day07 PUBLIC cxx_std_20)
SET_TARGET_PROPERTIES(day07 PROPERTIES CXX_EXTENSIONS OFF)
TARGET_COMPILE_OPTIONS(day07 PRIVATE "${WARNINGS}")

TARGET_COMPILE_FEATURES(day08 PUBLIC cxx_std_20)
SET_TARGET_PROPERTIES(day08 PROPERTIES CXX_EXTENSIONS OFF)
TARGET_COMPILE_OPTIONS(day08 PRIVATE "${WARNINGS}")

TARGET_COMPILE_FEATURES(day09 PUBLIC cxx_std_20)
SET_TARGET_PROPERTIES(day09 PROPERTIES CXX_EXTENSIONS OFF)
TARGET_COMPILE_OPTIONS(day09 PRIVATE "${WARNINGS}")

TARGET_COMPILE_FEATURES(day10 PUBLIC cxx_std_20)
SET_TARGET_PROPERTIES(day10 PROPERTIES CXX_EXTENSIONS OFF)
TARGET_COMPILE_OPTIONS(day10 PRIVATE "${WARNINGS}")

TARGET_COMPILE_FEATURES(day11 PUBLIC cxx_std_20)
SET_TARGET_PROPERTIES(day11 PROPERTIES CXX_EXTENSIONS OFF)
TARGET_COMPILE_OPTIONS(day11 PRIVATE "${WARNINGS}")

TARGET_COMPILE_FEATURES(day12 PUBLIC cxx_std_20)
SET_TARGET_PROPERTIES(day12 PROPERTIES CXX_EXTENSIONS OFF)
TARGET_COMPILE_OPTIONS(day12 PRIVATE "${WARNINGS}")

TARGET_COMPILE_FEATURES(day13 PUBLIC cxx_std_20)
SET_TARGET_PROPERTIES(day13 PROPERTIES CXX_EXTENSIONS OFF)
TARGET_COMPILE_OPTIONS(day13 PRIVATE "${WARNINGS}")

TARGET_COMPILE_FEATURES(day14 PUBLIC cxx_std_20)
SET_TARGET_PROPERTIES(day14 PROPERTIES CXX_EXTENSIONS OFF)
TARGET_COMPILE_OPTIONS(day14 PRIVATE "${WARNINGS}")

TARGET_COMPILE_FEATURES(solver.out PUBLIC cxx_std_20)
SET_TARGET_PROPERTIES(solver.out PROPERTIES CXX_EXTENSIONS OFF)
TARGET_COMPILE_OPTIONS(solver.out PRIVATE "${WARNINGS}")

TARGET_LINK_OPTIONS(solver.out PRIVATE "-fsanitize=address")
