#include "rv_test_helper.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <vector>

#include "cpu.h"

std::optional<std::string> RVHelper::generate_rv_assembly(
    const std::string& c_src) {
  std::string command = "riscv64-unknown-elf-gcc -S " + c_src + " -o ";
  int result = std::system(command.c_str());

  if (result != 0) {
    return std::nullopt;
  }
  return command;
}

std::optional<std::string> RVHelper::generate_rv_obj(
    const std::string& assembly) {
  size_t dotPos = assembly.find_last_of(".");
  std::string baseName =
      (dotPos == std::string::npos) ? assembly : assembly.substr(0, dotPos);

  std::string command = "riscv64-unknown-elf-gcc -Wl,-Ttext=0x0 -nostdlib -o " +
                        baseName + " " + assembly;
  int result = std::system(command.c_str());

  if (result != 0) {
    std::cerr << "Failed to generate RV object from assembly: " << assembly
              << std::endl;
    return std::nullopt;
  }
  return command;
}

std::optional<std::string> RVHelper::generate_rv_binary(
    const std::string& obj) {
  std::string command =
      "riscv64-unknown-elf-objcopy -O binary " + obj + " " + obj + ".bin";
  int result = std::system(command.c_str());

  if (result != 0) {
    std::cerr << "Failed to generate RV binary from object: " << obj
              << std::endl;
    return std::nullopt;
  }
  return command;
}

std::optional<Cpu> RVHelper::rv_helper(const std::string& code,
                                       const std::string& testname,
                                       size_t n_clock) {
  std::string filename = testname + ".s";
  std::ofstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Failed to create assembly file." << std::endl;
    return std::nullopt;
  }
  file << code;
  file.close();

  auto obj_result = generate_rv_obj(filename.c_str());
  if (!obj_result.has_value()) {
    return std::nullopt;
  }

  auto bin_result = generate_rv_binary(testname.c_str());
  if (!bin_result.has_value()) {
    return std::nullopt;
  }

  std::string binFilename = testname + ".bin";
  std::ifstream file_bin(binFilename, std::ios::binary);
  if (!file_bin.is_open()) {
    std::cerr << "Failed to open binary file." << std::endl;
    return std::nullopt;
  }

  std::vector<uint8_t> binaryCode((std::istreambuf_iterator<char>(file_bin)),
                                  std::istreambuf_iterator<char>());
  file_bin.close();

  Cpu cpu(binaryCode);
  for (size_t i = 0; i < n_clock; ++i) {
    uint64_t inst = cpu.fetch();
    auto new_pc = cpu.execute(inst);
    if (new_pc.has_value()) {
      cpu.pc = new_pc.value();
    } else {
      break;
    }
  }

  return cpu;
}
