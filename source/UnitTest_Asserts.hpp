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

#include <sstream>
#include "UnitTest_TestManager.hpp"

#ifndef _UNIT_TEST_ASSERTS_HPP_
#define _UNIT_TEST_ASSERTS_HPP_

namespace UnitTest {

   /*!
    * \brief
    * It contains asserts methods.
    */
   class Asserts {

   private:

      // It will be used to format error and messages.
      static std::stringstream ss;

   public:

      static inline void AssertFail(const char *message, const char *file, long line) {
        UnitTest::TestManager::display_error(UT_FUNCTION, file, line, message);
      }

      /*!
       * \def
       * AssertFail(MSG)
       *
       * \brief
       * It allows you to manually assert a case failed.
       *
       * \param MSG
       * The message to be displayed.
       **/
      #define AssertFail(MSG) UnitTest::Asserts::AssertFail(MSG, UT_TEST_CASE_LOCATION)


      static inline void AssertTrue(bool v, const char *file, long line) {
        if (!v) UnitTest::TestManager::display_error(UT_FUNCTION, file, line, "false");
      };

      /*!
       * \def
       * AssertTrue(BOOL_EXPR)
       *
       * \brief
       * It asserts that BOOL_EXPR evaluates to true.
       *
       * \param BOOL_EXPR
       * Any evaluable boolean expression.
       **/
      #define AssertTrue(BOOL_EXPR) UnitTest::Asserts::AssertTrue(BOOL_EXPR, UT_TEST_CASE_LOCATION)


      static inline void AssertFalse(bool v, const char *file, long line) {
        if (v) UnitTest::TestManager::display_error(UT_FUNCTION, file, line, "true");
      };

      /*!
       * \def
       * AssertFalse(BOOL_EXPR)
       *
       * \brief
       * It asserts that BOOL_EXPR evaluates to false.
       *
       * \param BOOL_EXPR
       * Any evaluable boolean expression.
       **/
      #define AssertFalse(BOOL_EXPR) UnitTest::Asserts::AssertFalse(BOOL_EXPR, UT_TEST_CASE_LOCATION)


      template<typename TX, typename TY>
      static inline void AssertEquals(TX tx, TY ty, const char *file, long line) {
        if (tx == ty) return;
        else {
           Asserts::ss << tx << " != " << ty;
           UnitTest::TestManager::display_error(UT_FUNCTION, file, line, Asserts::ss.str().c_str());
           Asserts::ss.str(std::string());
        }
      };

      /*!
       * \def
       * AssertEquals(LEFT_EXPR, RIGHT_EXPR)
       *
       * \brief
       * It asserts that LEFT_EXPR is == to RIGHT_EXPR.
       *
       * \param LEFT_EXPR
       * Any expression that returns a value.
       *
       * \param RIGHT_EXPR
       * Any expression that returns a value.
       **/
      #define AssertEquals(LEFT_EXPR, RIGHT_EXPR) UnitTest::Asserts::AssertEquals(LEFT_EXPR, RIGHT_EXPR, UT_TEST_CASE_LOCATION)


      template<typename TX, typename TY>
      static inline void AssertNotEquals(TX tx, TY ty, const char *file, long line) {
        if (tx != ty) return;
        else {
           Asserts::ss << tx << " == " << ty;
           UnitTest::TestManager::display_error(UT_FUNCTION, file, line, Asserts::ss.str().c_str());
           Asserts::ss.str(std::string());
        }
      }

      /*!
       * \def
       * AssertNotEquals(LEFT_EXPR, RIGHT_EXPR)
       *
       * \brief
       * It asserts that LEFT_EXPR is != to RIGHT_EXPR.
       *
       * \param LEFT_EXPR
       * Any expression that returns a value.
       *
       * \param RIGHT_EXPR
       * Any expression that returns a value.
       **/
      #define AssertNotEquals(LEFT_EXPR, RIGHT_EXPR) UnitTest::Asserts::AssertNotEquals(LEFT_EXPR, RIGHT_EXPR, UT_TEST_CASE_LOCATION)


      static inline void AssertThrows(bool ok, const char *expected, const char *found, const char *file, long line) {
        if (!ok) {
           Asserts::ss << expected << ", " << ((found != nullptr) ? found : "nothrow");
           UnitTest::TestManager::display_error(UT_FUNCTION, file, line, Asserts::ss.str().c_str());
           Asserts::ss.str(std::string());
        }
      }

      /*!
       * \def
       * AssertThrows(EXCEPTION_TYPE, EXPR)
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
       * other exceptions (using the ellipsis operator).
       **/
      #define AssertThrows(EXCEPTION_TYPE, EXPR) { \
               bool __ut_v = false; \
               bool __ut_c = false; \
               std::tuple<const char*, const char* > __ut_t; \
               try{ EXPR; } \
               catch (EXCEPTION_TYPE){ __ut_c = true; __ut_v = true, __ut_t = std::make_tuple(UT_COMPILE_TIME_STRING(EXCEPTION_TYPE), UT_COMPILE_TIME_STRING(EXCEPTION_TYPE)); } \
               catch (...){__ut_c = true; __ut_t = std::make_tuple(UT_COMPILE_TIME_STRING(EXCEPTION_TYPE), "unknown exception thrown"); } \
               if (!__ut_c){__ut_t = std::make_tuple(UT_COMPILE_TIME_STRING(EXCEPTION_TYPE),"nothrow");} \
               UnitTest::Asserts::AssertThrows(__ut_v, std::get<0>(__ut_t), std::get<1>(__ut_t), UT_TEST_CASE_LOCATION);}


         static inline void AssertNoThrows(bool ok, const char *thrown, const char *file, long line) {
           if (!ok) {
              Asserts::ss << "throw " << thrown;
              UnitTest::TestManager::display_error(UT_FUNCTION, file, line, Asserts::ss.str().c_str());
              Asserts::ss.str(std::string());
           }
         }

      /*!
       * \def
       * AssertNoThrows(EXPR)
       *
       * \brief
       * It asserts that the given EXPR does not throws any exception.
       *
       * \param EXPR
       * The expression that must be evaluated.
       *
       * \note
       * The actual implementation signals subclasses of std::exception like it does for
       * std::exception instead using their real types. The reason is that the expression
       * typeid(exception_obj).name() would returns a mangled name compiler dependent.
       **/
      #define AssertNoThrows(EXPR) { \
               bool thrown = false; \
               const char* thrown_e = nullptr; \
               try{ EXPR; } \
               catch(std::exception&){ thrown = true; thrown_e="std::exception";} \
               catch(...){thrown = true; thrown_e="unknown exception";} \
               (thrown)?(UnitTest::Asserts::AssertNoThrows(false, thrown_e, UT_TEST_CASE_LOCATION)): UnitTest::Asserts::AssertNoThrows(true, nullptr, nullptr, 0);}


      static void inline AssertLessTimeThan(bool val, double req, double found, const char *filename, long line) {
        if (!val) {
           Asserts::ss << req << ", " << found;
           UnitTest::TestManager::display_error(UT_FUNCTION, filename, line, Asserts::ss.str().c_str());
           Asserts::ss.str("");
        }
      }

      /*!
       * \def
       * AssertLessTimeThan(MAX_DOUBLE_INTERVAL, EXPR)
       *
       * \brief
       * It establishes the maximum time span required to evaluate EXPR.
       *
       * \param MAX_DOUBLE_INTERVAL
       * The maximum time span allowed.
       *
       * \param EXPR
       *  The expression to be executed.
       **/
      #define AssertLessTimeThan(MAX_DOUBLE_INTERVAL, EXPR) {\
            clock_t start, end; \
            start = clock();EXPR; end = clock();\
            double elapsed = (double(end -start)) / CLOCKS_PER_SEC; \
            double max_interval = (double)(MAX_DOUBLE_INTERVAL); \
            bool val = (max_interval >= elapsed); \
            UnitTest::Asserts::AssertLessTimeThan(val, max_interval, elapsed, UT_TEST_CASE_LOCATION); }


      static void inline AssertMoreTimeThan(bool val, double req, double found, const char *filename, long line) {
        if (!val) {
           Asserts::ss << req << ", " << found;
           UnitTest::TestManager::display_error(UT_FUNCTION, filename, line, Asserts::ss.str().c_str());
           Asserts::ss.str("");
        }
      }

      /*!
       * \def
       * AssertMoreTimeThan(MIN_DOUBLE_INTERVAL, EXPR)
       *
       * \brief
       * It establishes the minimum time span required to evaluate EXPR.
       *
       * \param MIN_DOUBLE_INTERVAL
       * The minimum time span allowed.
       *
       * \param EXPR
       * The expression to be executed.
       **/
      #define AssertMoreTimeThan(MIN_DOUBLE_INTERVAL, EXPR) {\
            clock_t start, end; \
            start = clock();EXPR; end = clock();\
            double elapsed = (double(end -start)) / CLOCKS_PER_SEC; \
            double min_interval = (double)(MIN_DOUBLE_INTERVAL); \
            bool val = (elapsed > min_interval); \
            UnitTest::Asserts::AssertMoreTimeThan(val, min_interval, elapsed, UT_TEST_CASE_LOCATION);}

    };
}

#endif // _UNIT_TEST_ASSERTS_HPP_
