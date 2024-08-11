#ifndef RV_TEST_HELPER_H
#define RV_TEST_HELPER_H
#include <optional>
#include <string>
#include <vector>

#include "cpu.h"

class Cpu;

class RVHelper {
 public:
  static std::optional<std::string> generate_rv_assembly(
      const std::string& c_src);
  static std::optional<std::string> generate_rv_obj(
      const std::string& assembly);
  static std::optional<std::string> generate_rv_binary(const std::string& obj);
  static std::optional<Cpu> rv_helper(const std::string& code,
                                      const std::string& testname,
                                      size_t n_clock);
};

#endif  // RV_TEST_HELPER_H
