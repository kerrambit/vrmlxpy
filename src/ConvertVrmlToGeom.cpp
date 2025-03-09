#include "vrmlxpy.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
  if (argc != 4) {
    std::cout << "Usage: " << argv[0] << " <input_file> <output_file> <config_file>" << std::endl;
    return -1;
  }

  vrmlxpy::PrintVersion();
  vrmlxpy::ConvertVrmlToGeom(argv[1], argv[2], argv[3]);

  return 0;
}
