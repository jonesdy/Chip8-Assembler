#include <iostream>
#include "Assembler.hpp"

int main(int argc, char *argv[])
{
   Assembler assembler;

   if(argc <= 1)
   {
      std::cout<<"Please input a filename\n";
   }
   else
   {
      assembler.assemble(argv[1]);
   }

   return 0;
}
