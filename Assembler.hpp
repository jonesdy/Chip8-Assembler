#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include <fstream>
#include <iostream>
#include <string>
#include "CodeLabel.hpp"

class Assembler
{
public:
   Assembler();
   void assemble(std::string fileName);
private:
};

#endif
