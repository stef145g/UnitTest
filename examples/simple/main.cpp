#include "UnitTest.hpp"
#include "Tests.hpp"

int main(int argc, char **argv, char **envp) {
   UT_REGISTER_TEST_SUITE(TestSuite);
   UnitTest::TestManager::run(argc, argv);
   return 0;
}
