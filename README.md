# UnitTest
A C++ (std11+) library to perform unit testing.

## How to?
Unit testing a piece of code is as simple as performing the following steps:

```c++
// Include the UnitTest library header
#include "UnitTest.hpp"

// This is supposed to be the unit under test
unsigned fact(unsigned n){ return (n==0 || n==1) ? 1 : n*fact(n-1); }

// This is how you declare a suite of test cases
UT_TEST_SUITE(test_fact) {

  // Here you can define variables that have global scope in the suite;
  unsigned result;
  
  // Here you can define code that will be executed before each test case
  UT_TEST_CASE_INIT() {
    // Inside here you can use variables with suite scope and wider
  }
  
  // Here you can define code that will be executed after each test case
  UT_TEST_CASE_CLEANUP() {
    // Inside here you can use variables with suite scope and wider
  }
  
  // Here is how you define test cases
  //  test cases must have a name
  //  test cases can have tags which enables you to select at runtime which test cases you want to execute
  UT_TEST_CASE(name, tag1, tag2){
    
  }
  
  // Note: following the name of test cases which does not have tags must be always a comma
  UT_TEST_CASE(another_name, ){
  
  }
  
  // After having define all test cases they must be enabled
  UT_ENABLE_TEST_CASES(
    name, 
    another_name
  );
}

int main(int argc, char** argv, char** envp){

  // test suite must be registered before execution
  UT_REGISTER_TEST_SUITE(test_fact);

  // this execute all the enabled tests
  UnitTest::TestManager::Run(argc, argv);
  
  return 0;
}
```
