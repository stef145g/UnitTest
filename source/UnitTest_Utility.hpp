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

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>


#ifndef _UNIT_TEST_UTILITY_HPP_
#define _UNIT_TEST_UTILITY_HPP_


namespace UnitTest {

    /*!
     * \brief
     * Class of utility methods.
     */
    class Utility {

    public:

        /*!
         * \brief
         * It splits the string s by delim and inserts the result in the vector v.
         *
         * \param[in] s
         * The string to be splitted.
         *
         * \param[in] delimiter
         * The delimiting char.
         *
         * \param[out] v
         * The vector to be filled.
         *
         * \param[in] strip_spaces
         * When true spaces are stripped by the end of the substrings.
         *
         * \returns
         * void.
         *
         * \remarks
         * It always removes empty strings.
         */
        static void split(std::string s, char delimiter, std::vector<std::string> &v, bool strip_spaces = true);

        /*!
         * \brief
         * It formats in a space of length characters the given filename and line values.
         *
         * \param[in] filename
         * The name of the file.
         *
         * \param[in] line
         * The line of the file.
         *
         * \param[in] length
         * The maximum character space to be used to format the given information.
         *
         * \return
         * It returns the string formatted as required when length is positive and greater
         * than fifteen, an empty string otherwise.
         */
        static std::string formatfinfo(const char *filename, long line, const int length = 70);

        /*!
         * \brief
         * It centers the given data string in a space of length characters.
         *
         * \param[in] data
         * The string to be centered.
         *
         * \param[i] length
         * The lenght of the space of characters in which the given data must be centered.
         *
         * \return
         * The centered string.
         **/
        static std::string center(const char *data, size_t length);
    };

}

#endif // _UNIT_TEST_UTILITY_HPP_
