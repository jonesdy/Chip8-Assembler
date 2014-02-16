#include "Assembler.hpp"

Assembler::Assembler()
{
}

void Assembler::assemble(const std::string &fileName)
{
   parse(fileName);

   // Go through the tokens
   // Check syntax and add labels first
   // Then go through again and actually assemble the code
   
   int numArgsNeeded = 0;
   bool needComma = false;
   unsigned int curMemLoc = 0;
   bool error = false;
   for(unsigned int i = 0; i < tokens.size() && !error; i++)
   {
      if(needComma)
      {
         if(tokens[i].compare(","))
         {
            std::cout<<"Error, expected ','"<<std::endl;
            error = true;
         }
         needComma = false;
      }
      else if(isCodeLabel(tokens[i]))
      {
         if(numArgsNeeded != 0)
         {
            std::cout<<"Error, expected argument\n";
            error = true;
         }
         // Found a code label, add it
         std::string label = tokens[i].substr(0, tokens[i].size() - 1);
         if(label.size())
         {
            if(getLabelLocation(label) == -1)
            {
               labels.push_back(CodeLabel(label, curMemLoc));
            }
            else
            {
               std::cout<<"Error, duplicate label\n";
               error = true;
            }
         }
         else
         {
            std::cout<<"Error, empty label\n";
            error = true;
         }
      }
      else
      {
         if(numArgsNeeded != 0)
         {
            // Make sure the next token isn't an instruction
            if(getNumArgs(tokens[i]) != -1)
            {
               std::cout<<"Error, argument expected\n";
               error = true;
            }
            else
            {
               // This should be an argument, so lets decrement the number we need
               numArgsNeeded--;

               // If we need any more, then make sure the next token is a comma
               if(numArgsNeeded)
               {
                  needComma = true;
               }
            }
         }
         else
         {
            // This should be an instruction
            numArgsNeeded = getNumArgs(tokens[i]);
            if(numArgsNeeded == -1)
            {
               std::cout<<"Error, instruction expected\n";
               error = true;
            }
            else
            {
               // Found an instruction, update the memory location
               curMemLoc += 2;
            }
         }
      }
   }

   if(numArgsNeeded)
   {
      std::cout<<"Error, argument expected\n";
      error = true;
   }

   for(unsigned int i = 0; i < labels.size(); i++)
   {
      std::cout<<labels[i].getLabelName()<<" "<<labels[i].getLocation()<<std::endl;
   }

   if(error)
   {
      std::cout<<"ERROR: Assembly failed (see above)\n";
   }
   else
   {
      // Time to actually do some assembly
      unsigned char memory[curMemLoc + 2];
      for(unsigned int i = 0; i < curMemLoc + 2; i++)
      {
         memory[i] = 0;
      }

      unsigned int ti = 0;
      for(unsigned int i = 0; i < curMemLoc + 2; i += 2)
      {
         if(isCodeLabel(tokens[ti]))
         {
            // Skip past code labels
            ti++;
         }
         if(!tokens[ti].compare("clr"))
         {
            // Clear, 00E0
            memory[i] = 0x00;
            memory[i + 1] = 0xE0;
            ti++;
         }
         else if(!tokens[ti].compare("ret"))
         {
            // Return, 00EE
            memory[i] = 0x00;
            memory[i + 1] = 0xEE;
            ti++;
         }
         else if(!tokens[ti].compare("jmp"))
         {
            // Jump, 1NNN
            std::string arg = tokens[++ti];
            // Should be a code label
            int loc = getLabelLocation(arg);
            if(loc == -1)
            {
               error = true;
               std::cout<<"Label "<<arg<<" does not exist."<<std::endl;
            }
            else
            {
               unsigned short opcode = 0x1000 | (loc & 0x0FFF);
               memory[i] = (opcode & 0xFF00) >> 8;
               memory[i + 1] = opcode & 0x00FF;
            }
            ti++;
         }
         else
         {
            ti++;
         }
      }

      // Write the memory to the file
      std::ofstream output((fileName + ".bin").c_str(), std::ofstream::binary);
      output.write((char*)memory, curMemLoc + 2);
      output.close();
   }
}

void Assembler::parse(const std::string &fileName)
{
   std::cout<<"File Name: "<<fileName<<std::endl;
   std::ifstream input(fileName.c_str());

   std::string line;
   while(input.good())
   {
      std::getline(input, line);
      std::vector<std::string> lineTokens = split(line);    // Split the line
      for(unsigned int i = 0; i < lineTokens.size(); i++)
      {
         tokens.push_back(lineTokens[i]);
      }
   }

   /*for(unsigned int i = 0; i < tokens.size(); i++)
   {
      std::cout<<tokens[i]<<" ";
   }
   std::cout<<std::endl;*/
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

int Assembler::getNumArgs(const std::string &in)
{
   if(!(in.compare("clr") && in.compare("ret")))
   {
      return 0;
   }
   else if(!(in.compare("jmp") && in.compare("call") && in.compare("shr") &&
            in.compare("shl") && in.compare("ind") && in.compare("jmp0") &&
            in.compare("kse") && in.compare("ksn") && in.compare("del") &&
            in.compare("wkp") && in.compare("setd") && in.compare("sets") &&
            in.compare("adi") && in.compare("spr") && in.compare("sbi") &&
            in.compare("sto") && in.compare("fil")))
   {
      return 1;
   }
   else if(!(in.compare("se") && in.compare("sne") && in.compare("set") &&
            in.compare("add") && in.compare("or") && in.compare("and") &&
            in.compare("xor") && in.compare("sub") && in.compare("subx") &&
            in.compare("rnd")))
   {
      return 2;
   }
   else if(!in.compare("draw"))
   {
      return 3;
   }
   else
   {
      // Instruction doesn't exist
      return -1;
   }
}

bool Assembler::isCodeLabel(const std::string &s)
{
   return(s[s.size() - 1] == ':');
}

int Assembler::getLabelLocation(const std::string &label)
{
   for(unsigned int i = 0; i < labels.size(); i++)
   {
      if(!label.compare(labels[i].getLabelName()))
      {
         return labels[i].getLocation();
      }
   }

   return -1;
}
