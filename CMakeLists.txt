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

SET(DAYS
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

FOREACH(DAY ${DAYS})
	ADD_LIBRARY(
		${DAY} SHARED
		AdventOfCode2024/day.hpp
		"AdventOfCode2024/${DAY}.cpp"
	)
	TARGET_LINK_LIBRARIES(${DAY} utils)
	TARGET_LINK_LIBRARIES(solver.out ${DAY})
ENDFOREACH()

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

FOREACH(DAY utils ${DAYS} solver.out)
	TARGET_COMPILE_FEATURES(${DAY} PUBLIC cxx_std_20)
	SET_TARGET_PROPERTIES(${DAY} PROPERTIES CXX_EXTENSIONS OFF)
	TARGET_COMPILE_OPTIONS(${DAY} PRIVATE "${WARNINGS}")
ENDFOREACH()

TARGET_LINK_OPTIONS(solver.out PRIVATE "-fsanitize=address")
