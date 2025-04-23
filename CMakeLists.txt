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
	day15
	day16
	day17
	day18
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

# Warnings for gcc version "Ubuntu 9.4.0-1ubuntu1~20.04.2"
# Found supported warning flags by looking in "info gcc"
# and subtracting out the flags enabled by -Wall, -Wextra, -Wpedantic
# by checking "gcc -Q -x c++ -Wall --help=warnings" (and for other warnings)

set(WARNINGS
	-Wall                         # enable many other warnings
	-Wextra                       # more warnings
	-Wpedantic                    # strict ISO C++
	-pedantic-errors              # depends on c++ standard used
	-Wno-aggregate-return
	-Waligned-new
	-Walloc-zero
	-Walloca
	-Wc++17-compat
	-Wcast-align
	-Wcast-align=strict
	-Wcast-qual                   # warn if casting removes type-qualifiers
	-Wconditionally-supported
	-Wconversion                  # warn about conversions that may change the value
	-Wdate-time                   # do not use __TIME__, __DATE__, or __TIMESTAMP__ to get reproducible builds
	-Wdisabled-optimization
	-Wdouble-promotion            # warn if float implicitly casts to double
	-Wduplicated-branches         # prevent branch statements that provide same value
	-Wduplicated-cond             # prevent same condition in if and else if
	-Wextra-semi
	-Wfloat-equal                 # do not use equality on floating-points
	-Wformat-contains-nul
	-Wformat-extra-args
	-Wformat=2
	-Wformat-overflow=1
	-Wformat-signedness
	-Wformat-truncation=1
	-Wframe-address
	-Winline
	-Winvalid-pch
	-Wlogical-op                  # warn about suspicious usages of logical operations ||, &&, ^
	-Wno-missing-braces           # -Wmissing-braces was removed from -Wall
	-Wmissing-format-attribute
	-Wmissing-include-dirs        # warn if include directory not valid
	-Wmissing-noreturn            # if we should have attribute [[noreturn]]
	-Wmultichar
	-Wnull-dereference
	-Wpacked
	-Wpacked-not-aligned
	-Wno-padded
	-Wredundant-decls             # warn when multiple declarations
	-Wshadow=global               # prevent shadowing of other variables
	-Wshift-overflow=2
	-Wsign-conversion             # warn about conversions between signed and unsigned integers
	-Wno-scalar-storage-order
	-Wstack-protector
	-Wstringop-truncation
	-Wsuggest-attribute=pure      # warn if some functions should have these attributes
	-Wsuggest-attribute=const
	-Wsuggest-attribute=noreturn
	-Wsuggest-attribute=format
	-Wsuggest-attribute=cold
	-Wsuggest-attribute=malloc
	-Wsuggest-final-types
	-Wsuggest-final-methods
	-Wsuggest-override
	-Wswitch-default              # case statements should have default case
	-Wswitch-enum
	-Wno-system-headers
	-Wtrampolines
	-Wundef
	-Wunused                      # all -Wunused-* warnings
	-Wunused-macros               # warn about macros that are not expanded anywhere
	-Wuseless-cast
	-Wvector-operation-performance
	-Wzero-as-null-pointer-constant
	-Wctor-dtor-privacy
	-Wdeprecated-copy-dtor
	-Wno-multiple-inheritance
	-Wno-namespaces
	-Wnoexcept
	-Wnon-virtual-dtor
	-Wregister
	-Weffc++
	-Wstrict-null-sentinel
	-Wno-templates
	-Wold-style-cast
	-Woverloaded-virtual
	-Wsign-promo
	-Wno-virtual-inheritance
	-Wstrict-overflow=2
	-Wno-missing-declarations
	-fno-operator-names           # do not use "and", "or", and "xor" (and others) as keywords
	-fno-common                   # prevent tentative definitions
	-fstack-protector-all         # stack protection
)

FOREACH(DAY utils ${DAYS} solver.out)
	TARGET_COMPILE_FEATURES(${DAY} PUBLIC cxx_std_20)
	SET_TARGET_PROPERTIES(${DAY} PROPERTIES CXX_EXTENSIONS OFF)
	TARGET_COMPILE_OPTIONS(${DAY} PRIVATE "${WARNINGS}")
ENDFOREACH()

TARGET_LINK_OPTIONS(solver.out PRIVATE "-fsanitize=address")
