#include "UnitUnderTest.hpp"
#include "UnitTest.hpp"

#include <iostream>

// The signature of the tested method
// void split(std::string s, char delim, std::vector<std::string>& v, bool strip_spaces = true);

UT_TEST_SUITE(Suites) {

   // Declare here variables
   std::string test_string;
   std::vector<std::string> splits;

   // It inits the variables shared between tests
   UT_TEST_CASE_INIT() {
                           test_string = "AAA , BBB, CCC, DDD ,";
                       };

   // It cleans the variables shared between tests
   UT_TEST_CASE_CLEANUP() {
                              test_string = "";
                              splits.clear();
                          };


   UT_TEST_CASE(With_default_white_space_stripping_and_empy_removed, empty) {
       UnitTest::Utility::split(test_string, ',', splits);
       AssertTrue(splits.size() == 4);

       for (auto it = std::begin(splits); it != std::end(splits); ++it) {
          AssertNotEquals((*it).at(0), ' ');
          AssertNotEquals((*it).at((*it).size() - 1), ' ');
       }
   };

   UT_TEST_CASE(With_white_space_not_stripped_and_empty_removed, empty) {
       UnitTest::Utility::split(test_string, ',', splits, false);
       AssertTrue(splits.size() == 4);

       AssertEquals(splits[0].at(splits[0].size() - 1), ' ');
       AssertEquals(splits[1].at(0), ' ');
       AssertEquals(splits[2].at(0), ' ');
       AssertEquals(splits[3].at(0), ' ');
       AssertEquals(splits[3].at(splits[3].size() - 1), ' ');
   };

   UT_TEST_CASE(With_a_string_of_separators, notEmpty) {
       test_string = ",,,,,,,,,,,,";
       UnitTest::Utility::split(test_string, ',', splits);
       AssertEquals(splits.size(), 0);
   };

   UT_TEST_CASE(With_a_string_of_separators_and_whitespaces_not_stripped, notEmpty) {
       test_string = ",,   ,,,,  ,,,  ,,  ,";
       UnitTest::Utility::split(test_string, ',', splits, false);
       AssertEquals(splits.size(), 4);

       AssertEquals(splits[0], "   ");
       AssertEquals(splits[1], "  ");
       AssertEquals(splits[2], "  ");
       AssertEquals(splits[3], "  ");
   };

   UT_TEST_CASE(With_an_empty_string, empty) {
       test_string = "";
       UnitTest::Utility::split(test_string, ',', splits);
       AssertEquals(splits.size(), 0);
   };

   UT_TEST_CASE(With_substrings_with_inner_spaces, empty) {
       test_string = "una stringa con spazi, un'altra string con spazi, ed ancora spazi,";
       UnitTest::Utility::split(test_string, ',', splits);
       AssertEquals(splits.size(), 3);

       for (auto it = std::begin(splits); it != std::end(splits); ++it) {
          AssertEquals((*it).find(' '), std::string::npos);
       }
   };

   UT_ENABLE_TEST_CASES(
      With_a_string_of_separators_and_whitespaces_not_stripped,
      With_default_white_space_stripping_and_empy_removed,
      With_white_space_not_stripped_and_empty_removed,
      With_substrings_with_inner_spaces,
      With_a_string_of_separators,
      With_an_empty_string
   );
};
