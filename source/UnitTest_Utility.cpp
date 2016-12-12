#include "UnitTest_Utility.hpp"

void UnitTest::Utility::split(std::string s, char delimiter, std::vector<std::string> &v, bool strip_spaces) {
   std::stringstream ss(s);
   std::string item;
   while (std::getline(ss, item, delimiter)) {
      if (strip_spaces)
         item.erase(std::remove_if(item.begin(), item.end(), isspace), item.end());
      if (item == "")
         continue;
      v.push_back(item);
   }
}

std::string UnitTest::Utility::center(const char *data, size_t length) {
   std::string _data = data;

   if (length <= _data.size()) {
      if (_data.size() > 0 && length > 0) {
         std::stringstream ss;
         ss << std::setw(static_cast<int>(length / 2)) << std::right << _data.substr(0, _data.size() / 2);
         ss << std::setw(static_cast<int>(length / 2)) << std::left
            << _data.substr(_data.size() / 2, std::string::npos);
         return ss.str();
      }

      return _data.substr(0, length);
   }

   return _data;
}

std::string UnitTest::Utility::formatfinfo(const char *filename, long line, const int length) {

   // by definition
   static const int min_length = 15;

   std::stringstream ss;
   std::string fname = filename;
   int lline = static_cast<int>(std::to_string((long) line).length());

   if (length < min_length) return "";
   int half_inner_length = (length - (lline + 4)) / 2;
   if ((int) fname.length() + lline < length) {
      ss << fname << ":" << line;
      return ss.str();
   }

   ss << fname.substr(static_cast<size_t>(0), static_cast<size_t>(half_inner_length)) << "..."
      << fname.substr(fname.length() - half_inner_length + 1, static_cast<size_t>(half_inner_length)) << ": " << line;
   return ss.str();
}
