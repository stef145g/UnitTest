/*
The MIT License(MIT)

Copyright(c) 2016-2017 Giovanni Lombardo

Permission is hereby granted, free of charge, to any person obtaining a copy of this
software and associated documentation files(the "Software"), to deal in the Software
without restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and / or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to the following
conditions :

The above copyright notice and this permission notice shall be included in all copies
or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

*/

#include <unordered_set>
#include <functional>
#include <sstream>
#include <vector>
#include <map>

#include "UnitTest_TestManager.hpp"
#include "UnitTest_Utility.hpp"
#include "UnitTest_Console.hpp"
#include "UnitTest_Requires.hpp"

// Static member initialization
UT_TEST_SUITE_LIST_TYPE UnitTest::TestManager::m_cases = UT_TEST_SUITE_LIST_TYPE();
std::map<std::function<void(void)> *, const char *> UnitTest::TestManager::m_names;
std::map<std::string, std::unordered_set<std::string>> UnitTest::TestManager::m_tests_tags;
std::map<const char *, int> UnitTest::TestManager::m_failed;
std::map<std::string, std::function<void(void)> *> UnitTest::TestManager::m_inits;
std::map<std::string, std::function<void(void)> *> UnitTest::TestManager::m_cleanup;
std::unordered_set<std::string> UnitTest::TestManager::m_enabled_tags;
bool UnitTest::TestManager::m_ret = true;
bool UnitTest::TestManager::m_filter_suites = false;
bool UnitTest::TestManager::m_filter_cases = false;
int UnitTest::TestManager::number_of_cases = 0;
int UnitTest::TestManager::m_number_of_executed_cases = 0;
const char *UnitTest::TestManager::current_case = nullptr;

void UnitTest::TestManager::display_error(
   const char *function,
   const char *file,
   long line,
   const char *value,
   bool print) {

   // It counts the number of asserts/requires failed for each case
   (UnitTest::TestManager::m_failed.count(UnitTest::TestManager::current_case) <= 0) ?
      UnitTest::TestManager::m_failed[UnitTest::TestManager::current_case] = 0 : // when true
      UnitTest::TestManager::m_failed[UnitTest::TestManager::current_case] += 1; // when false

   if (print) { // print is false for unhandled exceptions

      // It gets the name of the assert/require failed removing qualifications
      std::string fname = function;
      size_t pos = fname.find_last_of("::");
      fname = (pos != std::string::npos) ?
              fname.substr(pos + 1, fname.length() - pos + 2) :
              fname;

      // It prints the name of of the assert/require failed.
      std::stringstream ss;
      ss << "  " << fname << "(" << value << ")" << std::endl;
      ss << "    " << UnitTest::Utility::formatfinfo(file, line) << std::endl;
      std::string content = ss.str();
      ::Console::WriteError(content);
   }

   // It signals the failure of the current tests case
   TestManager::m_ret = false;
   return;
}

void UnitTest::TestManager::set_test_case_tags(std::string name, const char *tags) {

   UT_GET_NAME_VECTOR_FROM_VARARGS(vtags, tags);
   for (auto it = std::begin(vtags); it != std::end(vtags); ++it) {
      TestManager::m_tests_tags[name].insert(*it);
   }
}

bool UnitTest::TestManager::has_tag(std::string test_case) {

   // The filtering behaviour has not been required
   if (!TestManager::m_filter_cases) return true;

   // The current test case does not exists
   if (TestManager::m_tests_tags.count(test_case) <= 0) return false;

   // Check if the current test case has been tagged with the given tag
   for (auto it = std::begin(TestManager::m_tests_tags[test_case]);
        it != std::end(TestManager::m_tests_tags[test_case]); ++it) {
      if (TestManager::m_enabled_tags.count(*it) > 0)
         return true;
   }

   return false;
}

void UnitTest::TestManager::enable_test_suite(const char *name, void(*p)()) {
   UnitTest::TestManager::m_cases.push_back(std::make_tuple(name, p));
   return;
}

void UnitTest::TestManager::set_total_number_of_cases(size_t number_of_tests) {
   UnitTest::TestManager::number_of_cases = static_cast<int>(number_of_tests);
   return;
}

void UnitTest::TestManager::set_current_case(const char *name) {
   UnitTest::TestManager::current_case = name;
   return;
}

void UnitTest::TestManager::unhandled_exception(const char *filename, const char *function, long line) {
   UnitTest::TestManager::display_error(function, filename, line, " UNHANDLED EXCEPTION ", true);
   return;
}

void UnitTest::TestManager::set_init_method(std::string suite, std::function<void(void)> &_init) {
   UnitTest::TestManager::m_inits[suite] = &_init;
   return;
}

void UnitTest::TestManager::set_cleanup_method(std::string suite, std::function<void(void)> &_cleanup) {
   UnitTest::TestManager::m_cleanup[suite] = &_cleanup;
}

bool UnitTest::TestManager::get_init_method(std::string suite, std::function<void(void)> &_init) {
   if (UnitTest::TestManager::m_inits.count(suite) != 0) {
      _init = *(UnitTest::TestManager::m_inits[suite]);
      return true;
   }
   return false;
}

bool UnitTest::TestManager::get_cleanup_method(std::string suite, std::function<void(void)> &_cleanup) {
   if (UnitTest::TestManager::m_cleanup.count(suite) != 0) {
      _cleanup = *(UnitTest::TestManager::m_cleanup[suite]);
      return true;
   }
   return false;
}

void UnitTest::TestManager::run_cases(
   const char *function,
   const char *file,
   long line,
   std::function<void()> *cases,
   std::vector<std::string> &names) {

   UnitTest::TestManager::set_total_number_of_cases(names.size());
   UnitTest::TestManager::m_number_of_executed_cases = 0;
   std::function<void(void)> cases_init;
   std::function<void(void)> cases_cleanup;

   unsigned int f_it = 0;
   auto n_it = std::begin(names);
   while (n_it != std::end(names)) {
      if (!UnitTest::TestManager::has_tag(*n_it)) {
         ++f_it, ++n_it;
         continue;
      }
      ::Console::WriteDefault(" TEST CASE -> ");
      ::Console::WriteDefault(*n_it);
      ::Console::WriteDefault("\n");
      UnitTest::TestManager::set_current_case((*n_it).c_str());

      if (UnitTest::TestManager::get_init_method(function, cases_init)) cases_init();

      try { cases[f_it](); }
      catch (UnitTest::RequireFailed) {}
      catch (...) { UnitTest::TestManager::unhandled_exception(file, function, line); }
      ++UnitTest::TestManager::m_number_of_executed_cases;

      if (UnitTest::TestManager::get_cleanup_method(function, cases_cleanup)) cases_cleanup();

      ++f_it, ++n_it;
   }
}

void UnitTest::TestManager::write_suite_header(const char *suite_name) {

	static const char *LINE = "--------------------------------------------------------------------------";
	static std::stringstream ss;

	Console::WriteColor(LINE, COLOR(YELLOW));
	Console::Write("\r\n");

	ss << "TEST SUITE -> " << suite_name << std::endl;
	std::string content = UnitTest::Utility::center(ss.str().c_str(), 75);
	Console::WriteColor(content, COLOR(YELLOW));
	Console::Write("\r");

	Console::WriteColor(LINE, COLOR(YELLOW));
	Console::Write("\r\n");

	return;
}

void UnitTest::TestManager::write_suite_footer() {

	static const char *ARROW = "------------------------------------------------------------------------->";
	static std::string content;
	static std::stringstream ss;

	static auto write_response = [&]()->void{
		 Console::WriteColor(ARROW, COLOR(WHITE));
		 ss << std::setw(5) << std::right << ((UnitTest::TestManager::m_ret) ? "OK" : "KO") << std::endl;
		 content = ss.str();
		 (UnitTest::TestManager::m_ret) ? Console::Write(content) : Console::WriteError(content);
		 ss.str("");
	};

	static auto write_failed = [&]()->void{
		 ss << std::setw(10) << std::right << "Failed: " << std::setw(5) << std::left << UnitTest::TestManager::m_failed.size();
		 content = ss.str();
		 Console::WriteError(content);
		 ss.str("");
	};

	static auto write_passed = [&]()->void{
		 ss << std::setw(10) << std::right << "Passed: " << std::setw(5) << std::left << UnitTest::TestManager::m_number_of_executed_cases - UnitTest::TestManager::m_failed.size();
		 content = ss.str();
		 Console::Write(content);
		 ss.str("");
	};

	static auto write_total = [&]()->void{
		 ss << std::setw(10) << std::right << "Total: " << std::setw(5) << std::left << UnitTest::TestManager::m_number_of_executed_cases;
		 content = ss.str();
		 Console::WriteColor(content, COLOR(WHITE));
		 ss.str("");
	};

	write_response();
	write_failed();
	write_passed();
	write_total();

	std::cout << std::endl << std::endl;
	return;
}

void UnitTest::TestManager::run(int argc, char **argv) {

	const char *LINE = "--------------------------------------------------------------------------";
	const char *ARG_SUITES = "--suites";
	const char *ARG_TAGS = "--tags";
	const char *ARROW = "------------------------------------------------------------------------->";

	auto AcquireFollowers = [](char **argv, int idx, int limit, std::unordered_set<std::string> &follower) -> void {

		 std::string current;

		 while (true) {
			 if (idx == limit) break;
			 current = argv[idx];
			 if ((current.length() > 0 && current.at(0) == '-') || current == "") break;
			 follower.insert(current);
			 ++idx;
		 }
	};


	std::unordered_set<std::string> enabled_suites;
	for (int idx = 1; idx < argc; ++idx) {
		std::string current = argv[idx];

		if (current == ARG_SUITES) {
			TestManager::m_filter_suites = true;
			AcquireFollowers(argv, ++idx, argc, enabled_suites);
		}

		if (current == ARG_TAGS) {
			TestManager::m_filter_cases = true;
			AcquireFollowers(argv, ++idx, argc, TestManager::m_enabled_tags);
		}
	}

	UT_TEST_SUITE_LIST_TYPE_ITERATOR it = std::begin(UnitTest::TestManager::m_cases);
	::Console::InitConsole();
	for (; it != std::end(UnitTest::TestManager::m_cases); ++it) {

		if (TestManager::m_filter_suites) {
			if (enabled_suites.count(std::get<0>(*it)) == 0)
				continue;
		}

		write_suite_header(std::get<0>(*it));
		try { std::get<1>(*it)(); }
		catch (...) { m_ret = false; }
		write_suite_footer();

		// It cleans variables used to track UT_TEST_CASE's execution.
		UnitTest::TestManager::number_of_cases = 0;
		UnitTest::TestManager::m_failed.clear();
		m_ret = true;
	}
}

void UnitTest::TestManager::run() {
   TestManager::run(0, nullptr);
}
