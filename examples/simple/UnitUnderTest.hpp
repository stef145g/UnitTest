#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

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
void split(std::string s, char delimiter, std::vector<std::string> &v, bool strip_spaces) {
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
