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

#include "UnitTest_Console.hpp"

#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)

#define COLOR(X) #X

std::pair<const char *, const char *> Console::m_pairs[] = {
	std::pair<const char *, const char *>("RED", "\x1B[31m"),
	std::pair<const char *, const char *>("GREEN", "\x1B[32m"),
	std::pair<const char *, const char *>("YELLOW", "\x1B[33m"),
	std::pair<const char *, const char *>("BLUE", "\x1B[34m"),
	std::pair<const char *, const char *>("MAGENTA", "\x1B[35m"),
	std::pair<const char *, const char *>("CYAN", "\x1B[36m"),
	std::pair<const char *, const char *>("WHITE", "\x1B[37m"),
	std::pair<const char *, const char *>("RESET", "\x1B[0m"),
};

std::map<std::string, const char *> Console::m_colours;

bool Console::m_init = false;

#elif defined(_WIN32)

#define COLOR(X) X

// Handles
HANDLE Console::m_standardOutHandle = nullptr;
HANDLE Console::m_standardErrHandle = nullptr;
HANDLE Console::m_currentConsoleHandle = nullptr;

// Others
WORD Console::m_currentConsoleAttributes = 0;
CONSOLE_SCREEN_BUFFER_INFO Console::csbi;

bool Console::m_init = false;

#endif


