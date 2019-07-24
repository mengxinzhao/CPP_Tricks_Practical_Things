#include <stdio.h>
#include <fstream>
#include <iostream>
#include <regex>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
struct ParseModel {
  constexpr static char pattern[] = "model\\s*:\\s*\\d*";
  using return_type = std::vector<std::string>;
  return_type parse(std::string const &str) {
    std::regex re(pattern);
    std::smatch m;
    std::vector<std::string> matches;
    if (std::regex_search(str, m, re)) {
      for (size_t i = 0; i < m.size(); i++) {
        matches.push_back(m[i]);
      }
    }

    return matches;
  }
};

struct ParseVendor {
  constexpr static char pattern[] = "vendor_id\\s*:\\s*[a-zA-Z]*";
  using return_type = std::vector<std::string>;
  return_type parse(std::string const &str) {
    std::regex re(pattern);
    std::smatch m;
    std::vector<std::string> matches;
    if (std::regex_search(str, m, re)) {
      for (size_t i = 0; i < m.size(); i++) {
        matches.push_back(m[i]);
      }
    }

    return matches;
  }
};

template <typename... Callback>
std::tuple<typename Callback::return_type...> ReadFile(const char *filename,
                                                       Callback &&... callback) {
  using return_type = std::tuple<typename Callback::return_type...>;
  std::ifstream file;
  file.open(filename);

  if (!file.is_open()) {
    return return_type{};
  }
  // read all content here
  std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  // iterate through all callbacks.
  return return_type{callback.parse(content)...};
}

int main() {
  auto ret1 = ReadFile("./cpuinfo.txt", ParseModel{});
  auto ret2 = ReadFile("./cpuinfo.txt", ParseModel{}, ParseVendor{});
  auto model = std::get<0>(ret2);
  for (auto const &v : model) std::cout << v << std::endl;
  auto vendor = std::get<1>(ret2);
  for (auto const &v : vendor) std::cout << v << std::endl;
}
