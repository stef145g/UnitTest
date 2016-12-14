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

## What to I need to know to use it?
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
Test cases must be defined inside the scope of a test suite; each test case will have a name that, in this case also, must be a valid c++ identifier, and zero or more 'tags'. Tags are strings not delimited by double or single quotes and without spaces that allows you to classify the test case. Tags must be separated by commas. Using tags you can control at runtime which test cases of which test suites will be executed. When no tags are given for a test case a single comma after the test name must be used. As mentioned earlier variables defined at suite scope can be accessed inside test cases. Here is how you define test cases:
```c++
#include "UnitTest.hpp"

// This is how you define a UnitTest test suite dubbed suite_name
UT_TEST_SUITE(suite_name){

	// Here you can define variables visible at suite scope
	int iResult = 0;
	float fResult = 0.0f;
	double dResult = 0.0;
	
	UT_TEST_CASE(test_case_0,){
		// Here you actually unit test your code
	};
	
	UT_TEST_CASE(test_case_1, tag0, tag1, tagn){
		// Here you actually unit test your code
	};
	
}

```
Note that **the definition of each test case scope must ends with a semicolon**, also note that as previously said **test cases that does not have tags must have a colon at the end of their names**. 

Test cases can be enabled for execution. Specifically test cases that you want to be executed must be explicitly enabled. The order in which they are enabled will be the same order in which they will be executed, this will affect the runtime when you use Requires and/or test cases depend on each other. Here is how to enable test cases:
```c++
#include "UnitTest.hpp"

// This is how you define a UnitTest test suite dubbed suite_name
UT_TEST_SUITE(suite_name){

	// Here you can define variables visible at suite scope
	int iResult = 0;
	float fResult = 0.0f;
	double dResult = 0.0;
	
	UT_TEST_CASE(test_case_0,){
		// Here you actually unit test your code
	};
	
	UT_TEST_CASE(test_case_1, tag0, tag1, tagn){
		// Here you actually unit test your code
	};
	
	// Test cases must be explicitly enabled (order of execution will be the same)
	UT_ENABLE_TEST_CASES(
		test_case_0,
		test_case_1
	);
}

```
Some times it's required to execute pieces of code before each test case and/or at the end, in those cases UnitTest allows you to do so using **UT_TEST_CASE_INIT()** and **UT_TEST_CASE_CLEANUP()** as in the following code:
```c++
#include "UnitTest.hpp"

// This is how you define a UnitTest test suite dubbed suite_name
UT_TEST_SUITE(suite_name){

	// Here you can define variables visible at suite scope
	int iResult = 0;
	float fResult = 0.0f;
	double dResult = 0.0;
	
	UT_TEST_CASE_INIT(){
		//The code inserted here will be executed before each test case execution.
	};
	
	UT_TEST_CASE_CLEANUP(){
		// The code inserted here will be executed after each test case execution.
	};
	
	UT_TEST_CASE(test_case_0,){
		// Here you actually unit test your code
	};
	
	UT_TEST_CASE(test_case_1, tag0, tag1, tagn){
		// Here you actually unit test your code
	};
	
	// Test cases must be explicitly enabled (order of execution will be the same)
	UT_ENABLE_TEST_CASES(
		test_case_0,
		test_case_1
	);
}

```
Note that both **UT_TEST_CASE_INIT()** and **UT_TEST_CASE_CLEANUP()** definitions must end with a semicolon. Moreover they does not have names nor tags. When they are defined they will always be executed for each test case! 

Sometimes actions must be performed before the execution of all test cases and/or after their execution, using UnitTest you should perform those actions respectively before and after the **UT_ENABLE_TEST_CASES()** call. For the sake of readability we advise to put pre-actions after suite scope variables declaration and post-actions after the **UT_ENABLE_TEST_CASES()** call as in the following code:
```c++
#include "UnitTest.hpp"

// This is how you define a UnitTest test suite dubbed suite_name
UT_TEST_SUITE(suite_name){

	// Here you can define variables visible at suite scope
	int iResult = 0;
	float fResult = 0.0f;
	double dResult = 0.0;
	
	// The code inserted here will be executed once in the given suite and before all test cases!
	CreateDbConnection();
	
	UT_TEST_CASE_INIT(){
		//The code inserted here will be executed before each test case execution.
	};
	
	UT_TEST_CASE_CLEANUP(){
		// The code inserted here will be executed after each test case execution.
	};
	
	UT_TEST_CASE(test_case_0,){
		// Here you actually unit test your code
	};
	
	UT_TEST_CASE(test_case_1, tag0, tag1, tagn){
		// Here you actually unit test your code
	};
	
	// Test cases must be explicitly enabled (order of execution will be the same)
	UT_ENABLE_TEST_CASES(
		test_case_0,
		test_case_1
	);
	
	// The code inserted here will be executed once and after all test cases have been executed.
	DestroyDbConnection();
}

```
UnitTest has its own console test runner. It uses formatted and colored text to allow you visually have insigth on tests execution. To be executed by the test runner test suites must be registered. To schedule test suites for execution and exeute them you'll need to write code like the following:
```c++

// Include the UnitTest library header
#include "UnitTest.hpp"

// Inside MyTests.hpp there are two test suites: Suite1 and Suite2
#include "MyTests.hpp"

int main(int argc, char** argv){

	// Here we schedule for execution Suite1 and Suite2
	UT_REGISTER_TEST_SUITE(Suite1);
	UT_REGISTER_TEST_SUITE(Suite2);
	
	// Here we start the test runner
	UnitTest::TestManager::run(argc, argv);
	
	return 0;
}

```
After having compiled your tests you can select which from the enabled suites to schedule for execution mentioning them after the optional command line argument *--suites*. When this optional argument is not given all registered suites will be executed. Supposing having compiled a tester called *MyTester* to enable execution of the test suites Suite1 and Suite2 we could use the following command line:
```bash
./MyTester --suites Suite1 Suite2
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
