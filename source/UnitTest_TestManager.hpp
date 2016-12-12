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
#include <string>
#include <tuple>
#include <map>

#ifndef _UNIT_TEST_MANAGER_HPP_
#define _UNIT_TEST_MANAGER_HPP_

// Conversion to compile-time string
#define UT_COMPILE_TIME_STRING(_) #_

// The type used to define a UT_TEST_SUITE
#define UT_TEST_SUITE_TYPE void(*)()

// The type used to describe a UT_TEST_SUITE
#define UT_TEST_SUITE_DESCRIPTOR_TYPE std::tuple<const char*, UT_TEST_SUITE_TYPE>

// The type used as list of UT_TEST_SUITE descriptors
#define UT_TEST_SUITE_LIST_TYPE std::vector<UT_TEST_SUITE_DESCRIPTOR_TYPE>

// The type used as iterator for the list of UT_TEST_SUITE descriptors
#define UT_TEST_SUITE_LIST_TYPE_ITERATOR std::vector<UT_TEST_SUITE_DESCRIPTOR_TYPE>::iterator

// UT_TEST_SUITE declaration
#define UT_TEST_SUITE(SUITE) void SUITE()

// UT_TEST_CASE declaration where:
//		CASE is the name to assign to the UT_TEST_CASE
//		__VA_ARGS__ is the list of words to use to tag the UT_TEST_CASE
// 	NOTE: When __VA_ARGS__ is empty the comma after the name is mandatory
#define UT_TEST_CASE(CASE, ...) UnitTest::TestManager::set_test_case_tags(UT_COMPILE_TIME_STRING(CASE), #__VA_ARGS__); std::function<void(void)> CASE = [&]()->void

// Declaration of the initialization function of all UT_TEST_CASE of a UT_TEST_SUITE
#define UT_TEST_CASE_INIT() std::function<void(void)> _init; UnitTest::TestManager::set_init_method(__FUNCTION__, _init); _init = [&]()->void

// Declaration fo the cleanup function of all UT_TEST_CASE of a UT_TEST_SUITE
#define UT_TEST_CASE_CLEANUP() std::function<void(void)> _cleanup; UnitTest::TestManager::set_cleanup_method(__FUNCTION__, _cleanup); _cleanup = [&]()->void

// It creates a vector V containing all comma separated values in the ARGS string
#define UT_GET_NAME_VECTOR_FROM_VARARGS(V, ARGS) std::vector<std::string> V; UnitTest::Utility::split(ARGS,',', V);

// It enables each UT_TEST_CASE named in the CSV string __VA_ARGS__
#define UT_ENABLE_TEST_CASES(...) UT_GET_NAME_VECTOR_FROM_VARARGS(___ns, #__VA_ARGS__); std::function<void(void)> ___cs [] = {__VA_ARGS__}; UnitTest::TestManager::run_cases(__FUNCTION__, __FILE__, __LINE__, ___cs , ___ns);

// It requires a UT_TEST_SUITE execution
#define UT_REGISTER_TEST_SUITE(X) UnitTest::TestManager::enable_test_suite(UT_COMPILE_TIME_STRING(X), X)

// It can be used to track file and line of a piece of code
#define UT_TEST_CASE_LOCATION __FILE__, __LINE__

// It can be used to track the function of a piece of code
#define UT_FUNCTION __FUNCTION__


namespace UnitTest {

    /*!
     * \brief
     * It enables execution and management of UT_TEST_SUITE and UT_TEST_CASE.
     **/
    class TestManager {

    private:

        // It holds all UT_TEST_CASEs to execute in the given order.
        static UT_TEST_SUITE_LIST_TYPE m_cases;

        // It holds the names of all UT_TEST_CASEs.
        static std::map<std::function<void(void)> *, const char *> m_names;

        // It holds the tags of all UT_TEST_CASEs.
        static std::map<std::string, std::unordered_set<std::string>> m_tests_tags;

        // It holds the name of all failed UT_TEST_CASEs.
        static std::map<const char *, int> m_failed;

        // It tracks which UT_TEST_SUITE has a UT_INIT_TEST_CASE method.
        static std::map<std::string, std::function<void(void)> *> m_inits;

        // It tracks which UT_TEST_SUITE has a UT_CLEANUP_CASE method.
        static std::map<std::string, std::function<void(void)> *> m_cleanup;

        // It holds all enabled tags.
        static std::unordered_set<std::string> m_enabled_tags;

        // Used to tell whether one or more UT_TEST_CASES have been failed in a UT_TEST_SUITE.
        static bool m_ret;

        // It holds whether or not filtering behaviour of UT_TEST_SUITEs is enabled.
        static bool m_filter_suites;

        // It holds whether or not filtering behaviour of UT_TEST_CASEs is enabled.
        static bool m_filter_cases;

        // It stores the total number of UT_TEST_CASEs in the current UT_TEST_SUITE.
        static int number_of_cases;

        // It stores the number of UT_TEST_CASEs executed in the current UT_TEST_SUITE.
        static int m_number_of_executed_cases;

        // It holds the name of the UT_TEST_CASE currently in execution.
        static const char *current_case;

        // Used to establish that the Asserts class can have access to private members of the current class.
        friend class Asserts;

        // Used to establish that the Requires class can have access to private members of the current class.
        friend class Requires;


        /*!
         * \brief
         * It's used to displays that an error has happened in a Require or Assert method.
         *
         * \param[in] function
         * The name of the Assert or Required method that failed.
         *
         * \param[in] file
         * The name of the file in which is the method that failed.
         *
         * \param[in] line
         * The line of the file in which is the method that failed.
         *
         * \param[in] value
         * The value that generated the error.
         *
         * \returns
         * void.
         */
        static void display_error(const char *function, const char *file, long line, const char *value,
                                  bool print = true);

    public:

        /*!
         * \brief
         * It maps the test case dubbed name, with tags contained in tags.
         *
         * \param[in] name
         * The name of the UT_TEST_CASE.
         *
         * \param[in] tags
         * The CSV string of tags.
         *
         * \returns
         * Void.
         **/
        static void set_test_case_tags(std::string name, const char *tags);

        /*!
        * \brief
        * It returns a boolean value indicating whether the UT_TEST_CASE whose name is the
        * value of test_case variable must be executed or not.
        *
        * \param[in] test_case
        * The name of the test case to be checked.
        *
        * \returns
        * See the brief description.
        *
        * \notes
        * The semantic used is the following:
        *  - When the --tags command line parameter is not given, true is returned (no filtering behaviour).
        *  - When the given name is not found in the list of enabled UT_TEST_CASEs, false is returned.
        *  - True is returned only when the UT_TEST_CASE with the given name is found to have a tag from the ones
        *    specified in the --tags commmand line argument.
        **/
        static bool has_tag(std::string test_case);

        /*!
         * \brief
         * It enables the execution of the UT_TEST_SUITE with given name and defined by p.
         *
         * \param[in] name
         * The name of the UT_TEST_SUITE.
         *
         * \param[in] p
         * The pointer to the UT_TEST_SUITE.
         *
         * \notes
         * This method is not intended to be used by the end user. Altought it is
         * publicly reachable it represents an implementative detail. The
         * UT_REGISTER_TEST_SUITE() macro must be used with this meaning.
         */
        static void enable_test_suite(const char *name, void(*p)());

        /*!
         * \brief
         * It sets the total number of UT_TEST_CASEs inside the current UT_TEST_SUITE.
         *
         * \param[in] number_of_cases
         * The number of UT_TEST_CASEs.
         *
         * \returns
         * void.
         */
        static void set_total_number_of_cases(size_t number_of_cases);

        /*!
         * \brief
         * It sets the name of the UT_TEST_CASE currently executing.
         *
         * \param[in] name
         * The name of the current UT_TEST_CASE
         *
         * \returns
         * void.
         **/
        static void set_current_case(const char *name);

        /*!
         * \brief
         * It allows the signaling of an unhandled exception during UT_TEST_CASEs execution.
         *
         * \returns
         * void.
         */
        static void unhandled_exception(const char *filename, const char *function, long line);

        /*!
         * \brief
         * It sets the init function to be executed before each UT_TEST_CASE in the
         * UT_TEST_SUITE dubbed suite.
         *
         * \param[in] suite
         * The name of the UT_TEST_SUITE.
         *
         * \param[in] _init
         * The function to be used to initialize each UT_TEST_CASE in the given suite.
         *
         * \returns
         * void.
         */
        static void set_init_method(std::string suite, std::function<void(void)> &_init);

        /*!
         * \brief
         * It sets the cleanup function to be executed before each UT_TEST_CASE in the
         * UT_TEST_SUITE dubbed suite.
         *
         * \param[in] suite
         * The name of the UT_TEST_SUITE.
         *
         * \param[in] _cleanup
         * The function to be used to cleanup each UT_TEST_CASE in the given suite.
         *
         * \returns
         * void.
         */
        static void set_cleanup_method(std::string suite, std::function<void(void)> &_cleanup);

        /*!
         * \brief
         * It gets the initialization function associated to the UT_TEST_SUITE having
         * the given name in the _init output argument.
         *
         * \param[in] suite
         * The name of the UT_TEST_SUITE.
         *
         * \param[out] _init
         * The function object that will be filled with the reference to the init function.
         *
         * \returns
         * It returns true when a UT_INIT_TEST_CASE has been definedy for the UT_TEST_SUITE
         * with the given name, it returns false otherwise.
         */
        static bool get_init_method(std::string suite, std::function<void(void)> &_init);

        /*!
        * \brief
        * It gets the cleanup function associated to the UT_TEST_SUITE having
        * the given name in the _cleanup output argument.
        *
        * \param[in] suite
        * The name of the UT_TEST_SUITE.
        *
        * \param[out] _init
        * The function object that will be filled with the reference to the cleanup function.
        *
        * \returns
        * It returns true when a UT_CLEANUP_TEST_CASE has been definedy for the UT_TEST_SUITE
        * with the given name, it returns false otherwise.
        */
        static bool get_cleanup_method(std::string suite, std::function<void(void)> &_cleanup);

        /*!
         * \brief
         * It executes all enabled UT_TEST_CASEs of the UT_TEST_SUITE whoose name is given as
         * value of the function variable.
         *
         * \param[in] function
         * The name of the UT_TEST_SUITE.
         *
         * \param[in] file
         * The full path of the file in which the UT_TEST_SUITE has been defined.
         *
         * \param[in] line
         * The line in which the UT_TEST_SUITE has been defined.
         *
         * \param[in] cases
         * The vector with lambda defining UT_TEST_CASEs of the UT_TEST_SUITE.
         *
         * \param[in] names
         * The vector with the names of each UT_TEST_CASe in the UT_TEST_SUITE.
         *
         * \returns
         * Void.
         **/
        static void run_cases(
           const char *function,
           const char *file,
           long line,
           std::function<void()> *cases,
           std::vector<std::string> &name
        );

        /*!
         * \brief
         * It executes all enabled UT_TEST_CASEs of all required UT_TEST_SUITEs. When --suites and/or
         * --tags command line arguments are given, filtering behaviour is performed.
         *
         * \param[in] argc
         * The number of  command line arguments (it should be the argc argument given to the main).
         *
         * \param[in] argv
         * The vector of strings given to the command line (it should be the argv given to the main).
         *
         * \returns
         * Void.
         **/
        static void run(int argc, char **argv);

        /*!
        * \brief
        * It executes all enabled UT_TEST_CASEs of each registered UT_TEST_SUITE.
        *
        * \returns
        * Void.
        */
        static void run();

        /*!
         * \brief
         * It writes to the console the header for the given suite name.
         *
         * \param[in] suite_name
         * The name of the test suite.
         */
        static void write_suite_header(const char *suite_name);

        /*!
         * \brief
         * It writes to the console the footer for the current suite.
         */
        static void write_suite_footer();
    };

}

#endif // _UNIT_TEST_MANAGER_HPP_
