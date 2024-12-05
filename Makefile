# Makefile style from https://codingnest.com/basic-makefiles/
SOURCES = AdventOfCode2024/main.cpp
SOURCES += AdventOfCode2024/utils.cpp
SOURCES += AdventOfCode2024/day01.cpp
SOURCES += AdventOfCode2024/day02.cpp
SOURCES += AdventOfCode2024/day03.cpp
SOURCES += AdventOfCode2024/day04.cpp

OBJECTS = $(SOURCES:.cpp=.o)

# additional compile flags
# see https://cliutils.gitlab.io/modern-cmake/chapters/features/cpp11.html
# enable many other warnings
WARNINGS = -Wall
# more warnings
WARNINGS += -Wextra
# strict ISO C++
WARNINGS += -Wpedantic
# depends on c++ standard used
WARNINGS += -pedantic-errors
# initalizers have proper braces
WARNINGS += -Wmissing-braces
# if include directory not valid
WARNINGS += -Wmissing-include-dirs
# case statements should have default case
WARNINGS += -Wswitch-default
# all -Wunused-* warnings
WARNINGS += -Wunused
# use without initialization
WARNINGS += -Wuninitialized
# do not use unknown #pragma
WARNINGS += -Wunknown-pragmas
# if we should have attribute [[noreturn]]
WARNINGS += -Wmissing-noreturn
# warn if some functions should have these attributes
WARNINGS += -Wsuggest-attribute=pure
WARNINGS += -Wsuggest-attribute=const
WARNINGS += -Wsuggest-attribute=noreturn
WARNINGS += -Wsuggest-attribute=malloc
# prevent comparing boolean with int
WARNINGS += -Wbool-compare
# prevent branch statements that provide same value
WARNINGS += -Wduplicated-branches
# prevent same condition in if and else if
WARNINGS += -Wduplicated-cond
# warn if float implicitly casts to double
WARNINGS += -Wdouble-promotion
# do not use equality on floating-points
WARNINGS += -Wfloat-equal
# prevent shadowing of local variables
WARNINGS += -Wshadow
# warn if compiler cannot optimize loop
WARNINGS += -Wunsafe-loop-optimizations
# warn about macros that are not expanded anywhere
WARNINGS += -Wunused-macros
# warn if casting removes type-qualifiers
WARNINGS += -Wcast-qual
# warn about conversions that may change the value
WARNINGS += -Wconversion
# do not use __TIME__, __DATE__, or __TIMESTAMP__ to get reproducible builds
WARNINGS += -Wdate-time
# warn about conversions between signed and unsigned integers
WARNINGS += -Wsign-conversion
# warn about suspicious usages of logical operations ||, &&, ^
WARNINGS += -Wlogical-op
# warn when multiple declarations
WARNINGS += -Wredundant-decls
# do not use "and", "or", and "xor" (and others) as keywords
WARNINGS += -fno-operator-names
# prevent tentative definitions
WARNINGS += -fno-common
# detect memory access errors
WARNINGS += -fsanitize=address
# stack protection
WARNINGS += -fstack-protector-all

CXXFLAGS += -std=c++2a $(WARNINGS) -MMD -MP

.PHONY: clean all
.DEFAULT_GOAL := all

all: solver.out

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ -c

solver.out: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f solver.out $(OBJECTS) $(OBJECTS:.o=.d)

-include $(OBJECTS:.o=.d)
