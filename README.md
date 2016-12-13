# UnitTest
A C++ (std11+) library to perform unit testing.

## How to?
Unit testing a piece of code is as simple as performing the following steps:

```c++
// Include the UnitTest library header
#include "UnitTest.hpp"

#include <limits>

// This are supposed to be the units under test
unsigned fact(unsigned n){ return (n == 0 || n == 1) ? 1 : n*fact(n - 1); }
unsigned fib(unsigned n){ return (n <= 1) ? 1 : fib(n - 1) + fib(n - 2); }

// This is how you define a suite of test cases
UT_TEST_SUITE(test_fact) {

	// Here you can define variables that have global scope in the suite;
	unsigned result;

	UT_TEST_CASE_INIT() {
		result = 0; // Here you can access variables at suite scope
	};

	UT_TEST_CASE_CLEANUP() {
		result = std::numeric_limits<unsigned>::max(); // Here you can access variables at suite scope
	};

  // Each case must have a name (a valid identifier) and zero or more tags: notes that after the name a comma is always required!
	UT_TEST_CASE(fact_of_zero_or_one_is_one, base){
		AssertEquals(result, 0);
		AssertEquals(fact(0), 1);
		AssertEquals(fact(1), 1);
	};

	UT_TEST_CASE(fact_of_five_is_correct, compute){
		AssertFalse(result != 0);
		AssertEquals(fact(5), 120);
	};

  // Test cases must be explicitly enabled
	UT_ENABLE_TEST_CASES(
		fact_of_zero_or_one_is_one,
		fact_of_five_is_correct
	);
}

UT_TEST_SUITE(test_fib){
	unsigned result;

	UT_TEST_CASE_INIT() {
		result = 0;
	};

	UT_TEST_CASE_CLEANUP() {
		result = std::numeric_limits<unsigned>::max();
	};

	UT_TEST_CASE(fib_of_one_gives_one, base){
		AssertEquals(1, fib(1));
	};

	UT_TEST_CASE(fib_of_zero_gives_one, base){
		AssertTrue(fib(0) == 1);
	};

	UT_TEST_CASE(fib_of_five_is_correct, compute){
		AssertEquals(fib(5), 8);
	};

	UT_ENABLE_TEST_CASES(
		fib_of_one_gives_one,
		fib_of_zero_gives_one,
		fib_of_five_is_correct
	);
}

int main(int argc, char **argv, char **envp) {

   UT_REGISTER_TEST_SUITE(test_fact);
   UT_REGISTER_TEST_SUITE(test_fib);

   // You can execute only one or more suite by giving --suites <suite 1> [suite 2] [suite n] in the command line
   // You can execute only one or more test cases tagged by giving --tags <tag 1> [tag 2] [tag n] in the command line
   UnitTest::TestManager::run(argc, argv);

   return 0;
}
```
