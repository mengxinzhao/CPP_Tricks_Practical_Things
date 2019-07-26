#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <regex>
#include <string_view>
#include <tuple>
#include <type_traits>
//#include <type_traits>

std::vector<std::string> internal_parse(std::string const &str, std::regex const &re) {
  std::vector<std::string> ret;
  for (std::sregex_iterator i = std::sregex_iterator(str.begin(), str.end(), re);
       i != std::sregex_iterator(); ++i) {
    std::smatch m = *i;
    ret.push_back(m[1].str());
  }
  return ret;
}


template <typename... Callback>
std::tuple<typename Callback::ReturnType...> ReadFile(const char *filename,
                                                      Callback &&... callback) {
  using ReturnType = std::tuple<typename Callback::ReturnType...>;
  std::ifstream file;
  file.open(filename);

  if (!file.is_open()) {
    return ReturnType{};
  }
  // read all content here
  std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  // iterate through all callbacks.
  return ReturnType{callback.parse(content)...};
}

typedef uint32_t UInt;
typedef std::string String;
typedef double Double;

inline uint32_t StringToUInt(const std::string &str) { return strtoull(str.c_str(), NULL, 16); }
inline double StringToDouble(const std::string &str) { std::stod(str); }
inline std::string StringToString(const std::string &str) { return str; }

#define DEFINE_CPUINFO_PARSER(name, pattern, return_type) \
  struct Parser##name {                                          \
    constexpr static char _pattern[] = pattern;                  \
    using ElementType = return_type;                             \
    using ReturnType = std::vector<ElementType>;                 \
    ReturnType parse(std::string const &str) {                   \
      auto matches = internal_parse(str, std::regex(_pattern, std::regex::ECMAScript));  \
      ReturnType ret;                                            \
      for (auto &elem : matches) {                               \
        ret.push_back(StringTo##return_type(elem));              \
      }                                                          \
      return ret;                                                \
    }                                                            \
  };
DEFINE_CPUINFO_PARSER(Model, "model\\s*:\\s*(\\d+)", UInt)
DEFINE_CPUINFO_PARSER(Vendor, "vendor_id\\s*:\\s*([a-zA-Z]*)", String)
DEFINE_CPUINFO_PARSER(Frequency, "cpu MHz\\s*:\\s*(\\d+.\\d*)", Double)
DEFINE_CPUINFO_PARSER(Microcode, "microcode\\s*:\\s*(0x[0-9a-f]*)", UInt)
int main() {
  // auto ret1 = ReadFile("./cpuinfo.txt", ParseModel{});
  // auto ret2 = ReadFile("./cpuinfo.txt", ParseModel{}, ParseVendor{});
  // auto model = std::get<0>(ret2);
  // for (auto const &v : model) std::cout << v << std::endl;
  // auto vendor = std::get<1>(ret2);
  // for (auto const &v : vendor) std::cout << v << std::endl;
  auto ret3 = ReadFile("./cpuinfo.txt", ParserVendor{}, ParserFrequency{}, ParserMicrocode{});
  auto vendor = std::get<0>(ret3);
  for (auto const &v : vendor) std::cout << v << std::endl;
  auto freq = std::get<1>(ret3);
  for (auto const &v : freq) printf("%f\n", v);
  auto microcode = std::get<2>(ret3);
  for (auto const &v : microcode) printf("0x%x\n", v);
}
