#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "CodeLabel.hpp"

class Assembler
{
public:
   Assembler();
   void assemble(std::string fileName);
private:
   std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
   std::vector<std::string> split(const std::string &s, char delim);
   std::vector<CodeLabel> labels;
};

#endif
