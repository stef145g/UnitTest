# UnitTest
A c++ (std11+) library to perform unit testing.

## Why do I need to use UnitTest?
Unit testing on source code allows you to give proof to your customers that your code works as expected under the given conditions and constraints. Moreover, in the software life cycle, bug fixing and maintenance often require editing being made to the source code: being able to automatically verify that the code continues to work as expected becomes very important. The UnitTest library is designed to be as easy as possible to use without renouncing to flexibility.

## How do I include it in my projects? 
To include UnitTest in your projects you just need to download the source code of the library, put it in a folder that can be accessed by the compiler and include its header file where you want to use it:

```c++

// This will let you use the UnitTest library in your projects
#include "UnitTest.hpp"

...
```

## How do I use it?
In UnitTest test cases are grouped under test suites. Each test suite must have a name which must also be a valid c++ identifier (it cannot start with a number, cannot contains spaces and so on). Test suites actually creates a scope, so each variable defined in the test suite can be used in the test cases of that test suite. Here is how you define a test suite:
```c++
#include "UnitTest.hpp"

// This is how you define a UnitTest test suite dubbed suite_name
UT_TEST_SUITE(suite_name){

	// Here you can define variables visible at suite scope
	int iResult = 0;
	float fResult = 0.0f;
	double dResult = 0.0;
	
}

```


## Simple usage
Unit testing a piece of C++ (std11+) code is as simple as follows:

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
	
		// Inside test cases you should use AssertXXX or RequireXXX macros to test your code
		
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
