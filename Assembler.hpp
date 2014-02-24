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
   void assemble(const std::string &fileName);
private:
   void parse(const std::string &fileName);
   std::vector<std::string> split(const std::string &s);
   int getNumArgs(const std::string &in);
   bool isCodeLabel(const std::string &s);
   int getLabelLocation(const std::string &label);
   int getRegister(const std::string &reg);
   int getConstant(const std::string &con);
   std::vector<std::string> tokens;
   std::vector<CodeLabel> labels;
};

#endif
