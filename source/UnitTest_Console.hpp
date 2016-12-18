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

#include <iostream>
#include <random>
#include <map>

#ifndef _UNIT_TEST_CONSOLE_HPP_
#define _UNIT_TEST_CONSOLE_HPP_

#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)

#define COLOR(X) #X

class Console {

private:

    // This will hold all the known color codes
    static std::map<std::string, const char *> m_colours;

    // The vector of
    static std::pair<const char *, const char *> m_pairs[];

    // Init
    static bool m_init;

    static int RandomColorIndex() {

       static const int min = 0;
       static const int max = 7;

       static std::random_device rd;
       static std::mt19937 rng(rd());
       static std::uniform_int_distribution<int> uni(min, max);

       return uni(rng);
    }

    static const char *RandomColorKey() {
       return Console::m_pairs[RandomColorIndex()].first;
    }

public:

    static bool InitConsole() {
       if (!Console::m_init) {
          for (short idx = 0; idx < 8; ++idx) Console::m_colours.insert(Console::m_pairs[idx]);
		 }
		 Console::m_init = true;
       return Console::m_init;
    }

    template<typename T>
    static void WriteError(T& msg) {
       std::cout << Console::m_colours["RED"] << msg << Console::m_colours["RESET"] << std::flush;
       return;
    }

    template<typename T>
    static void Write(T& msg) {
       std::cout << Console::m_colours["GREEN"] << msg << Console::m_colours["RESET"] << std::flush;
       return;
    }

    template<typename T>
    static void WriteDefault(T& msg) {
       std::cout << Console::m_colours["RESET"] << msg << std::flush;
       return;
    }

    template<typename T>
    static void WriteColor(T& msg, const char *color) {
       if (Console::m_colours.count(color) > 0) {
          std::cout << m_colours[color];
       }
       std::cout << msg << Console::m_colours["RESET"] << std::flush;
       return;
    }

    template<typename T>
    static void WriteRandom(T& msg) {
       std::cout << Console::m_colours[Console::RandomColorKey()] << msg << Console::m_colours["RESET"] << std::flush;
       return;
    }

    static void Reset() {
       std::cout << Console::m_colours["RESET"] << std::flush;
       return;
    }
};

#elif defined(_WIN32)

#define COLOR(X) X

#include "Windows.h"

/*!
* \brief
* Definisce le costanti colore disponibili per il foreground dell console.
* [in ambito windows]
**/

enum Color{
   BLACK,
   DARK_BLUE,
   DARK_GREEN,
   DARK_CYAN,
   DARK_RED,
   DARK_PURPLE,
   DARK_YELLOW,
   DARK_WHITE,
   GRAY,
   BLUE,
   GREEN,
   ACQUA,
   RED,
   PURPLE,
   YELLOW,
   WHITE,
};

static std::map<::Color, WORD> _colors_map;

class Console{

private:

   // Handles
   static HANDLE m_standardOutHandle;
   static HANDLE m_standardErrHandle;
   static HANDLE m_currentConsoleHandle;

   // Others
   static WORD m_currentConsoleAttributes;
   static CONSOLE_SCREEN_BUFFER_INFO csbi;

   // Init
   static bool m_init;


   static enum ::Color RandomColor(){

      static const int min = BLACK;
      static const int max = WHITE;

      // Initialize (seed) engine
      std::random_device rd;

      // Random-number engine used (Mersenne-Twister)
      std::mt19937 rng(rd());

      std::uniform_int_distribution<int> uni(min, max);
      return static_cast<::Color>(uni(rng));
   }

public:

   static bool InitConsole(){
      Console::InitColors();
      Console::m_init = false;
      Console::m_standardOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
      Console::m_standardErrHandle = GetStdHandle(STD_ERROR_HANDLE);
      if (Console::m_standardOutHandle == INVALID_HANDLE_VALUE || Console::m_standardErrHandle == INVALID_HANDLE_VALUE)
         return Console::m_init;

      Console::m_currentConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
      if (GetConsoleScreenBufferInfo(Console::m_currentConsoleHandle, &csbi)){
         Console::m_currentConsoleAttributes = Console::csbi.wAttributes;
         Console::m_init = true;
      }

      return Console::m_init;
   }

   static void InitColors(){
      ::_colors_map[::BLACK] = 0x0 % 16;
      ::_colors_map[::DARK_BLUE] = 0x1 % 16;
      ::_colors_map[::DARK_GREEN] = 0x2 % 16;
      ::_colors_map[::DARK_CYAN] = 0x3 % 16;
      ::_colors_map[::DARK_RED] = 0x4 % 16;
      ::_colors_map[::DARK_PURPLE] = 0x5 % 16;
      ::_colors_map[::DARK_YELLOW] = 0x6 % 16;
      ::_colors_map[::DARK_WHITE] = 0x7 % 16;
      ::_colors_map[::GRAY] = 0x8 % 16;
      ::_colors_map[::BLUE] = 0x9 % 16;
      ::_colors_map[::GREEN] = 0xA % 16;
      ::_colors_map[::ACQUA] = 0xB % 16;
      ::_colors_map[::RED] = 0xC % 16;
      ::_colors_map[::PURPLE] = 0xD % 16;
      ::_colors_map[::YELLOW] = 0xE % 16;
      ::_colors_map[::WHITE] = 0xF % 16;
   }

   template<typename T>
   static void WriteError(T& msg){
      SetConsoleTextAttribute(
         Console::m_standardErrHandle,
         FOREGROUND_INTENSITY | FOREGROUND_RED
         );
      std::cerr <<msg;
      Console::Reset();
   }

   template<typename T>
   static void Write(T& msg){
      SetConsoleTextAttribute(
         Console::m_standardOutHandle,
         FOREGROUND_INTENSITY | FOREGROUND_GREEN
         );
      std::cout << msg;
      Console::Reset();
   }

   template<typename T>
   static void WriteColor(T& msg, ::Color color){
      SetConsoleTextAttribute(
         Console::m_standardOutHandle,
         ::_colors_map[color]
         );
      std::cout << msg;
      Console::Reset();
   }

   template<typename T>
   static void WriteRandom(T& msg){
      SetConsoleTextAttribute(
         Console::m_standardOutHandle,
         ::_colors_map[Console::RandomColor()]
         );
      std::cout << msg;
      Console::Reset();
   }

   template<typename T>
   static void WriteDefault(T& msg){
      Console::Reset();
      std::cout << msg;
   }

   static void Reset(){
      SetConsoleTextAttribute(
         Console::m_currentConsoleHandle,
         m_currentConsoleAttributes
         );
   }
};

#endif // PLATFORM IDENTIFICATION

#endif // _UNIT_TEST_CONSOLE_HPP_
