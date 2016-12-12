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

#include "UnitTest_TestManager.hpp"
#include "UnitTest_Shared.hpp"

#ifndef _UNIT_TEST_REQUIRES_HPP_
#define _UNIT_TEST_REQUIRES_HPP_

namespace UnitTest {

   /*!
    * \brief
    * It contains Requires methods.
    */
   class Requires {

   private:

      // It will be used to format error and messages.
      static std::stringstream ss;

   public:

      static inline bool RequireTrue(bool v, const char *file, long line) {
        if (v) return true;
        UnitTest::TestManager::display_error(UT_FUNCTION, file, line, "false");
        throw UnitTest::RequireFailed();
      };

      /*!
       * \def
       * RequireTrue(BOOL_EXPR)
       *
       * \brief
       * It asserts that BOOL_EXPR evaluates to true. When this requirement is not
       * satisfied the current UT_TEST_CASE ends immediately.
       *
       * \param BOOL_EXPR
       * Any evaluable boolean expression.
       **/
      #define RequireTrue(BOOL_EXPR) UnitTest::Requires::RequireTrue(BOOL_EXPR, UT_TEST_CASE_LOCATION)


      static inline bool RequireFalse(bool v, const char *file, long line) {
        if (!v) return true;
        UnitTest::TestManager::display_error(UT_FUNCTION, file, line, "true");
        throw UnitTest::RequireFailed();
      };

      /*!
       * \def
       * RequireFalse(BOOL_EXPR)
       *
       * \brief
       * It asserts that BOOL_EXPR evaluates to false.When this requirement is not
       * satisfied the current UT_TEST_CASE ends immediately.
       *
       * \param BOOL_EXPR
       * Any evaluable boolean expression.
       **/
      #define RequireFalse(BOOL_EXPR) UnitTest::Requires::RequireFalse(BOOL_EXPR, UT_TEST_CASE_LOCATION)


      template<typename TX, typename TY>
      static inline void RequireEquals(TX tx, TY ty, const char *file, long line) {
        if (tx != ty) {
           Requires::ss << tx << " != " << ty;
           UnitTest::TestManager::display_error(UT_FUNCTION, file, line, Requires::ss.str().c_str());
           Requires::ss.str(std::string());
           throw UnitTest::RequireFailed();
        }
      };

      /*!
       * \def
       * AssertEquals(LEFT_EXPR, RIGHT_EXPR)
       *
       * \brief
       * It asserts that LEFT_EXPR is == to RIGHT_EXPR. When this requirement is not
       * satisfied the current UT_TEST_CASE ends immediately.
       *
       * \param LEFT_EXPR
       * Any expression that returns a value.
       *
       * \param RIGHT_EXPR
       * Any expression that returns a value.
       **/
      #define RequireEquals(LEFT_EXPR, RIGHT_EXPR) UnitTest::Requires::RequireEquals(LEFT_EXPR, RIGHT_EXPR, UT_TEST_CASE_LOCATION)


      template<typename TX, typename TY>
      static inline void RequireNotEquals(TX tx, TY ty, const char *file, long line) {
        if (tx == ty) {
           Requires::ss << tx << " == " << ty;
           UnitTest::TestManager::display_error(UT_FUNCTION, file, line, Requires::ss.str().c_str());
           Requires::ss.str(std::string());
           throw UnitTest::RequireFailed();
        }
      }

      /*!
       * \def
       * RequireNotEquals(LEFT_EXPR, RIGHT_EXPR)
       *
       * \brief
       * It asserts that LEFT_EXPR is != to RIGHT_EXPR.  When this requirement is not
       * satisfied the current UT_TEST_CASE ends immediately.
       *
       * \param LEFT_EXPR
       * Any expression that returns a value.
       *
       * \param RIGHT_EXPR
       * Any expression that returns a value.
       **/
      #define RequireNotEquals(LEFT_EXPR, RIGHT_EXPR) UnitTest::Requires::RequireNotEquals(LEFT_EXPR, RIGHT_EXPR, UT_TEST_CASE_LOCATION)


      static inline void
      RequireThrows(bool ok, const char *expected, const char *found, const char *file, long line) {
        if (!ok) {
           Requires::ss << expected << ", " << ((found != nullptr) ? found : "nothrow");
           UnitTest::TestManager::display_error(UT_FUNCTION, file, line, Requires::ss.str().c_str());
           Requires::ss.str(std::string());
           throw UnitTest::RequireFailed();
        }
      }

      /*!
       * \def
       * RequireThrows(EXCEPTION_TYPE, EXPR)
       *
       * \brief
       * It assert that EXPR throws an exception of the given EXCEPTION_TYPE.
       *
       * \param EXCEPTION_TYPE
       * The type of the exception that will be thrown.
       *
       * \param EXPR
       * Any expression.
       *
       * \note
       * Actually, the implementation isn't able to distinguish the type of standard
       * exceptions. This is because when EXCEPTION_TYPE is std::exception having a
       * catch clause for std::exception would cause a compilation error. So the
       * implementation has two catch clauses: one for the type given and one for all
       * other exceptions (using the ellipsis operator).When this requirement is not
       * satisfied the current UT_TEST_CASE ends immediately.
       **/
      #define RequireThrows(EXCEPTION_TYPE, EXPR) { \
            bool __ut_v = false; \
            bool __ut_c = false; \
            std::tuple<const char*, const char* > __ut_t; \
            try{ EXPR; } \
            catch (EXCEPTION_TYPE){ __ut_c = true; __ut_v = true, __ut_t = std::make_tuple(UT_COMPILE_TIME_STRING(EXCEPTION_TYPE), UT_COMPILE_TIME_STRING(EXCEPTION_TYPE)); } \
            catch (...){__ut_c = true; __ut_t = std::make_tuple(UT_COMPILE_TIME_STRING(EXCEPTION_TYPE), "unknown exception thrown"); } \
            if (!__ut_c){__ut_t = std::make_tuple(UT_COMPILE_TIME_STRING(X),"nothrow");} \
            UnitTest::Requires::RequireThrows(__ut_v, std::get<0>(__ut_t), std::get<1>(__ut_t), UT_TEST_CASE_LOCATION);}


      static inline void RequireNoThrows(bool ok, const char *thrown, const char *file, long line) {
        if (!ok) {
           Requires::ss << "throw " << thrown;
           UnitTest::TestManager::display_error(UT_FUNCTION, file, line, Requires::ss.str().c_str());
           Requires::ss.str(std::string());
           throw UnitTest::RequireFailed();
        }
      }

      /*!
       * \def
       * RequireNoThrows(EXPR)
       *
       * \brief
       * It asserts that the given EXPR does not throws any exception. When this requirement is not
       * satisfied the current UT_TEST_CASE ends immediately.
       *
       * \param EXPR
       * The expression that must be evaluated.
       *
       * \note
       * The actual implementation signals subclasses of std::exception like it does for
       * std::exception instead using their real types. The reason is that the expression
       * typeid(exception_obj).name() would returns a mangled name compiler dependent.
       **/
      #define RequireNoThrows(EXPR) { \
               bool thrown = false; \
               const char* thrown_e = nullptr; \
               try{ EXPR; } \
               catch(std::exception& e){ thrown = true; thrown_e="std::exception";} \
               catch(...){thrown = true; thrown_e="unknown exception";} \
               (thrown)?(UnitTest::Requires::RequireNoThrows(false, thrown_e, UT_TEST_CASE_LOCATION)): UnitTest::Requires::RequireNoThrows(true, nullptr, nullptr, 0);}


      static void inline RequireLessTimeThan(bool v, double req, double elapsed, const char *file, long line) {
        if (!v) {
           Requires::ss << req << ", " << elapsed;
           UnitTest::TestManager::display_error(UT_FUNCTION, file, line, Requires::ss.str().c_str());
           Requires::ss.str("");
           throw UnitTest::RequireFailed();
        }
      }

      /*!
       * \def
       * RequireLessTimeThan(MAX_DOUBLE_INTERVAL, EXPR)
       *
       * \brief
       * It establishes the maximum time span required to evaluate EXPR. When this requirement is not
       * satisfied the current UT_TEST_CASE ends immediately.
       *
       * \param MAX_DOUBLE_INTERVAL
       * The maximum time span allowed.
       *
       * \param EXPR
       * The expression to be executed.
       **/
      #define RequireLessTimeThan(MAX_DOUBLE_INTERVAL, Y) {\
            clock_t start, end; \
            start = clock();EXPR; end = clock();\
            double elapsed = (double(end -start)) / CLOCKS_PER_SEC; \
            double max_interval = (double)(MAX_DOUBLE_INTERVAL); \
            bool val = (max_interval > elapsed); \
            UnitTest::Requires::RequireLessTimeThan(val, max_interval, elapsed, UT_TEST_CASE_LOCATION);}


      static inline void RequireMoreTimeThan(bool v, double req, double elapsed, const char *file, long line) {
        if (!v) {
           Requires::ss << req << ", " << elapsed;
           UnitTest::TestManager::display_error(UT_FUNCTION, file, line, Requires::ss.str().c_str());
           Requires::ss.str("");
           throw UnitTest::RequireFailed();
        }
      }

      /*!
       * \def
       * RequireMoreTimeThan(MIN_DOUBLE_INTERVAL, EXPR)
       *
       * \brief
       * It establishes the minimum time span required to evaluate EXPR.When this requirement is not
       * satisfied the current UT_TEST_CASE ends immediately.
       *
       * \param MIN_DOUBLE_INTERVAL
       * The minimum time span allowed.
       *
       * \param EXPR
       * The expression to be executed.
       **/
      #define RequireMoreTimeThan(MIN_DOUBLE_INTERVAL, EXPR) {\
            clock_t start, end; \
            start = clock();EXPR; end = clock();\
            double elapsed = (double(end -start)) / CLOCKS_PER_SEC; \
            double min_interval = (double)(MIN_DOUBLE_INTERVAL); \
            bool val = (elapsed > min_interval); \
            UnitTest::Requires::RequireMoreTimeThan(val, min_interval, elapsed, UT_TEST_CASE_LOCATION);}

   };

}

#endif // _UNIT_TEST_REQUIRES_HPP_
