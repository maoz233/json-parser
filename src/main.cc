/**
 * @file main.cc
 * @author Mao Zhang (mao.zhang233@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-05-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <exception>
#include <iostream>

int main(int argc, char** argv) {
  if (!argc || !argv) {
    return EXIT_FAILURE;
  }

  try {
    } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}