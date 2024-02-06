#ifndef GENERATE_RV_H
#define GENERATE_RV_H

#include <gtest/gtest.h>
#include <fstream>
#include "../src/cup.h"
namespace cemu {
    void generate_rv_assembly(const std::string& c_src);
    void generate_rv_obj(const std::string& assembly);
    void generate_rv_binary(const std::string& obj);
    Cpu rv_helper(const std::string& code, const std::string& testname, size_t n_clock);
}
#endif  // GENERATE_RV_H
