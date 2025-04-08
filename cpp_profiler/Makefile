PROGRAMS = test_load_greedy test_greedy test_greedy_errors test_load_radix test_radix test_radix_errors test_radix_prefix

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 

all: $(PROGRAMS)

test_load_greedy: 
	$(CXX) $(CXXFLAGS) test_load_greedy.cpp greedy.cpp -o test_load_greedy

test_greedy: 
	$(CXX) $(CXXFLAGS) test_greedy.cpp greedy.cpp -o test_greedy

test_greedy_errors: 
	$(CXX) $(CXXFLAGS) test_greedy_errors.cpp greedy.cpp -o test_greedy_errors

test_load_radix: 
	$(CXX) $(CXXFLAGS) test_load_radix.cpp radix.cpp greedy.cpp -o test_load_radix

test_radix: 
	$(CXX) $(CXXFLAGS) test_radix.cpp radix.cpp greedy.cpp -o test_radix

test_radix_errors: 
	$(CXX) $(CXXFLAGS) test_radix_errors.cpp radix.cpp greedy.cpp -o test_radix_errors

test_radix_prefix: 
	$(CXX) $(CXXFLAGS) test_radix_prefix.cpp radix.cpp greedy.cpp -o test_radix_prefix


clean:
 rm -f *.exe

