# CXX Make variable for compiler
CXX=g++
# Make variable for compiler options
#	-std=c++11  C/C++ variant to use, e.g. C++ 2011
#	-g          include information for symbolic debugger e.g. gdb 
CXXFLAGS=-std=c++11 -g

# Rules format:
# target : dependency1 dependency2 ... dependencyN
#     Command to make target, uses default rules if not specified

# First target is the one executed if you just type make
# make target specifies a specific target
# $^ is an example of a special variable.  It substitutes all dependencies
dicttest : dicttree.o countwords.o populatetree.o dicttest.o -lpthread
	$(CXX) $(CXXFLAGS) -o dicttest $^


dicttree.o:		dicttree.h dicttree.cpp
countwords.o:	dicttree.h countwords.cpp
populatetree.o:	dicttree.h populatetree.cpp
dicttest.o: 		dicttree.h dicttest.cpp

clean :
	rm *.o

