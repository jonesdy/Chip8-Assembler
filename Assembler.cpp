#include "Assembler.hpp"

Assembler::Assembler()
{
}

void Assembler::assemble(std::string fileName)
{
   std::cout<<"File Name: "<<fileName<<std::endl;
   std::ifstream input(fileName.c_str());

   std::string line;
   std::vector<std::string> tokens;
   bool hasComment;
   while(input.good())
   {
      hasComment = false;
      std::getline(input, line);
      tokens = split(line, ' ');    // Split with spaces first
      for(unsigned int i = 0; i < tokens.size() && !hasComment; i++)
      {
         std::cout<<tokens[i]<<" ";
         // If we find a comment, we need to stop assembling this line
         if(tokens[i].find(';') != std::string::npos)
            hasComment = true;
      }
      std::cout<<std::endl;
   }
}

// Seriously, no decent string tokenizers in STL?
// Thanks https://stackoverflow.com/questions/236129/how-to-split-a-string-in-c
std::vector<std::string> &Assembler::split(const std::string &s, char delim, std::vector<std::string> &elems)
{
   std::stringstream ss(s);
   std::string item;
   while(std::getline(ss, item, delim))
   {
      elems.push_back(item);
   }
   return elems;
}

std::vector<std::string> Assembler::split(const std::string &s, char delim)
{
   std::vector<std::string> elems;
   split(s, delim, elems);
   return elems;
}
