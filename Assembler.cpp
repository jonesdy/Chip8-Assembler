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
   while(input.good())
   {
      std::getline(input, line);
      std::vector<std::string> lineTokens = split(line);    // Split the line
      for(unsigned int i = 0; i < lineTokens.size(); i++)
      {
         tokens.push_back(lineTokens[i]);
      }
   }

   for(unsigned int i = 0; i < tokens.size(); i++)
   {
      std::cout<<tokens[i]<<" ";
   }
   std::cout<<std::endl;
}

std::vector<std::string> Assembler::split(const std::string &s)
{
   // Split up the line with spaces and commas
   // Don't return anything after a semi-colon
   std::vector<std::string> tokens;
   std::string currentToken = "";
   for(unsigned int i = 0; i < s.length(); i++)
   {
      if(s[i] == ';')
      {
         // Found a comment, don't need to continue on this line
         if(currentToken.length() > 0)
         {
            tokens.push_back(currentToken);
         }
         return tokens;
      }
      else if(s[i] == ',')
      {
         // If there's a comma, put it into its own token and continue
         if(currentToken.length() > 0)
         {
            tokens.push_back(currentToken);
            currentToken = "";
         }
         tokens.push_back(",");
      }
      else if((s[i] != ' ') && (s[i] != '\t') && (s[i] != '\n') && (s[i] != '\r'))
      {
         currentToken += s[i];
      }
      else if(currentToken.length() > 0)
      {
         tokens.push_back(currentToken);
         currentToken = "";
      }
   }

   if(currentToken.length() > 0)
   {
      tokens.push_back(currentToken);
   }

   return tokens;
}
