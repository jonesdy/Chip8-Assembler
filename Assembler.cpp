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
            std::cout<<"Error, expected argument"<<std::endl;
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
               std::cout<<"Error, duplicate label"<<std::endl;
               error = true;
            }
         }
         else
         {
            std::cout<<"Error, empty label"<<std::endl;
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
               std::cout<<"Error, argument expected"<<std::endl;
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
               std::cout<<"Error, instruction expected"<<std::endl;
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
      std::cout<<"Error, argument expected"<<std::endl;
      error = true;
   }

   for(unsigned int i = 0; i < labels.size(); i++)
   {
      std::cout<<labels[i].getLabelName()<<" "<<labels[i].getLocation()<<std::endl;
   }

   if(error)
   {
      std::cout<<"ERROR: Assembly failed (see above)"<<std::endl;
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
      for(unsigned int i = 0; i < curMemLoc + 2 && !error; i += 2)
      {
         unsigned short opcode = 0;
         if(isCodeLabel(tokens[ti]))
         {
            // Skip past code labels
            ti++;
         }
         if(!tokens[ti].compare("clr"))
         {
            // Clear, 00E0
            opcode = 0x00E0;
            ti++;
         }
         else if(!tokens[ti].compare("ret"))
         {
            // Return, 00EE
            opcode = 0x00EE;
            ti++;
         }
         else if(!tokens[ti].compare("jmp"))
         {
            // Jump to NNN, 1NNN
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
               opcode = 0x1000 | (loc & 0x0FFF);
            }
            ti++;
         }
         else if(!tokens[ti].compare("call"))
         {
            // Call subroutine at NNN, 2NNN
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
               opcode = 0x1000 | (loc & 0x0FFF);
            }
            ti++;
         }
         else if(!tokens[ti].compare("ind"))
         {
            // Sets I to address NNN, ANNN
         }
         else if(!tokens[ti].compare("jmp0"))
         {
            // Jumps to the address NNN plus V0, BNN
         }
         else if(!tokens[ti].compare("kse"))
         {
            // Skips the next instruction if the key stored in VX is pressed
            // EX9E
         }
         else if(!tokens[ti].compare("ksn"))
         {
            // Skips the next instruction if the key stored in VX isn't pressed
            // EXA1
         }
         else if(!tokens[ti].compare("del"))
         {
            // Sets VX to the value of the delay timer, FX07
         }
         else if(!tokens[ti].compare("wkp"))
         {
            // A key press is awaited, and then stored in VX, FX0A
         }
         else if(!tokens[ti].compare("setd"))
         {
            // Sets the delay timer to VX, FX15
         }
         else if(!tokens[ti].compare("sets"))
         {
            // Sets the sound timer to VX, FX18
         }
         else if(!tokens[ti].compare("adi"))
         {
            // Adds VX to I, FX1E
         }
         else if(!tokens[ti].compare("spr"))
         {
            // Sets I to the location of the sprite for the character in VX
            // FX29
         }
         else if(!tokens[ti].compare("sbi"))
         {
            // Stores the binary-coded decimal respresentation of VX at I, I + 1, and I + 2
            // FX33
         }
         else if(!tokens[ti].compare("sto"))
         {
            // Stores V0 to VX in memory starting at address I, FX55
         }
         else if(!tokens[ti].compare("fil"))
         {
            // Fills V0 to VX with values from memory starting at address I, FX65
         }
         else if(!tokens[ti].compare("se"))
         {
            // Skips the next instruction if vx == nn, or if vx == vy
            // 3XNN or 5XY0
         }
         else if(!tokens[ti].compare("sne"))
         {
            // Skips the next instruction if vx != nn, or if vx != vy
            // 4XNN or 9XY0
         }
         else if(!tokens[ti].compare("set"))
         {
            // Sets VX to NN, or VX to the value of VY
            // 6XNN or 8XY0
         }
         else if(!tokens[ti].compare("add"))
         {
            // Adds NN to VX, or VX += VY
            // 7XNN or 8XY4
         }
         else if(!tokens[ti].compare("or"))
         {
            // VX = VX | VY
            // 8XY1
         }
         else if(!tokens[ti].compare("and"))
         {
            // VX = VX & VY
            // 8XY2
         }
         else if(!tokens[ti].compare("xor"))
         {
            // VX = VX ^ VY
            // 8XY3
         }
         else if(!tokens[ti].compare("sub"))
         {
            // VX -= VY
            // 8XY5
         }
         else if(!tokens[ti].compare("subx"))
         {
            // VX = VY - VX
            // 8XY7
         }
         else if(!tokens[ti].compare("rnd"))
         {
            // Sets VX to a random number and NN
            // CXNN
         }
         else if(!tokens[ti].compare("shr"))
         {
            // Shift vy right by 1 and store in vx
            // VF is set to the value of the LSB of VX before shift
            // 8XY6
            std::string arg1 = tokens[++ti];
            ti++;    // Should be a comma
            std::string arg2 = tokens[++ti];

            // Should be registers
            int reg1 = getRegister(arg1);
            int reg2 = getRegister(arg2);
            if(reg1 == -1)
            {
               error = true;
               std::cout<<"Arg "<<arg1<<" is not a valid register"<<std::endl;
            }
            else if(reg2 == -1)
            {
               error = true;
               std::cout<<"Arg "<<arg2<<" is not a valid register"<<std::endl;
            }
            else
            {
               opcode = 0x8006 | (reg1 << 8) | (reg2 << 4);
               std::cout<<std::hex<<"0x"<<opcode<<std::endl;
            }
            ti++;
         }
         else if(!tokens[ti].compare("shl"))
         {
            // Shift vy left by 1 and store in vx
            // VF is set to the value of the MSB of VX before shift
            // 8XYE
            std::string arg1 = tokens[++ti];
            ti++;    // Should be a comma
            std::string arg2 = tokens[++ti];

            // Should be registers
            int reg1 = getRegister(arg1);
            int reg2 = getRegister(arg2);
            if(reg1 == -1)
            {
               error = true;
               std::cout<<"Arg "<<arg1<<" is not a valid regsiter"<<std::endl;
            }
            else if(reg2 == -1)
            {
               error = true;
               std::cout<<"Arg "<<arg2<<" is not a valid register"<<std::endl;
            }
            else
            {
               opcode = 0x800E | (reg1 << 8) | (reg2 << 4);
               std::cout<<std::hex<<"0x"<<opcode<<std::endl;
            }
            ti++;
         }
         else if(!tokens[ti].compare("draw"))
         {
            // Draw sprites at coordinates VX, VY, with a height of N, width of 8,
            // sprite starts at I
            // DXYN
         }
         else
         {
            std::cout<<"Error"<<std::endl;
            error = true;
         }

         if(!error)
         {
            memory[i] = (opcode & 0xFF00) >> 8;
            memory[i + 1] = opcode & 0x00FF;
         }
      }

      // Write the memory to the file
      if(!error)
      {
         std::ofstream output((fileName + ".bin").c_str(), std::ofstream::binary);
         output.write((char*)memory, curMemLoc + 2);
         output.close();
      }
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
   else if(!(in.compare("jmp") && in.compare("call") && in.compare("ind") &&
            in.compare("jmp0") && in.compare("kse") && in.compare("ksn") &&
            in.compare("del") && in.compare("wkp") && in.compare("setd") &&
            in.compare("sets") && in.compare("adi") && in.compare("spr") &&
            in.compare("sbi") && in.compare("sto") && in.compare("fil")))
   {
      return 1;
   }
   else if(!(in.compare("se") && in.compare("sne") && in.compare("set") &&
            in.compare("add") && in.compare("or") && in.compare("and") &&
            in.compare("xor") && in.compare("sub") && in.compare("subx") &&
            in.compare("rnd") && in.compare("shr") && in.compare("shl")))
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

int Assembler::getRegister(const std::string &reg)
{
   // Should be V0 - VF
   if((reg.size() != 2) || (reg[0] != 'v'))
   {
      return -1;
   }
   
   switch(reg[1])
   {
      case '0':
         return 0;
      case '1':
         return 1;
      case '2':
         return 2;
      case '3':
         return 3;
      case '4':
         return 4;
      case '5':
         return 5;
      case '6':
         return 6;
      case '7':
         return 7;
      case '8':
         return 8;
      case '9':
         return 9;
      case 'a':
         return 0xA;
      case 'b':
         return 0xB;
      case 'c':
         return 0xC;
      case 'd':
         return 0xD;
      case 'e':
         return 0xE;
      case 'f':
         return 0xF;
      default:
         return -1;
   }
}
