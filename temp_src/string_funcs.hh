#ifndef STRING_FUNCS_HH
#define STRING_FUNCS_HH

#include <sstream>
#include <string>

namespace assetmarket {
template <typename T>
std::string to_string(const T& value) {
  std::ostringstream ss;
  ss << value;
  return ss.str();
}

template <typename T>
void from_string(const std::string& str, T& value) {
  std::istringstream ss(str);
  ss >> value;
}

}  // namespace assetmarket

#endif  // STRING_FUNCS_HH
