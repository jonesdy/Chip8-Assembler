#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iterator>
#include "CodeLabel.hpp"

const int NUM_INSTRUCTIONS = 30;
const int ONE_ARG_INSTRUCTIONS = 2;
const int TWO_ARG_INSTRUCTIONS = 19;
const int THREE_ARG_INSTRUCTIONS = 29;

const char *GOOD_INSTRUCTIONS[NUM_INSTRUCTIONS] =
{"clr", "ret", "jmp", "call", "shr", "shl", "ind", "jmp0", "kse", "ksn",
 "del", "wkp", "setd", "sets", "adi", "spr", "sbi", "sto", "fil", "se",
 "sne", "set", "add", "or", "and", "xor", "sub", "subx", "rnd", "draw"};

// se and set have two possible parameter sets
const enum {CLR, RET, JMP, CALL, SHR, SHL, IND, JMP0, KSE, KSN,
            DEL, WKP, SETD, SETS, ADI, SPR, SBI, STO, FIL, SE,
            SNE, SET, ADD, OR, AND, XOR, SUB, SUBX, RND, DRAW};

void skipWhiteSpace(char *&buf)
{

   while((*buf == ' ') || (*buf == '\n') || (*buf == '\t') || (*buf == '\r'))
      buf++;

}

char *getToken(char *&buf)
{

   int tokenLength = 0;
   char *bufPtr = buf;
   while((*buf != NULL) && (*buf != ' ') && (*buf != '\n') && (*buf != '\t')
      && (*buf != '\r') && (*buf != ',') && (*buf != ':') && (*buf != ';'))
   {
      tokenLength++;
      buf++;
   }

   if(*buf != NULL)     // Go past the thing we ended on, unless it's NULL
   {
      if((*buf == ';') || (*buf == ':') || (*buf == ','))
         tokenLength++;       // If we ended in a comment, label, or comma,
                              // we want it to be in the token
      buf++;
   }

   char *tok = new char[tokenLength + 1];
   for(int i = 0; i < tokenLength; i++)
      tok[i] = bufPtr[i];

   tok[tokenLength] = NULL;   // End in null

   return tok;

}

void skipComment(char *&buf)
{

   while((*buf != NULL) && (*buf != '\n'))
      buf++;

   if(*buf != NULL)
      buf++;      // Go past the newline

}

int getNumArgs(char *instruction)
{

   int args = -1;

   for(int i = 0; i < NUM_INSTRUCTIONS; i++)
   {
      if(!strcmp(instruction, GOOD_INSTRUCTIONS[i]))
      {
         if(i < ONE_ARG_INSTRUCTIONS)
            return 0;
         else if(i < TWO_ARG_INSTRUCTIONS)
            return 1;
         else if(i < THREE_ARG_INSTRUCTIONS)
            return 2;
         else
            return 3;
      }
   }

   return -1;

}

int main()
{

   std::ifstream input;

   // Read in binary mode to ignore pesky file encoding
   input.open("test.txt", std::ios::binary);

   int length;

   input.seekg(0, input.end);
   length = input.tellg();
   input.seekg(0, input.beg);
   char *buffer = new char[length + 1];
   input.read(buffer, length);
   input.close();
   buffer[length] = NULL;

   // Parse the buffer
   char *parseBuffer = buffer;
   int memLocation = 0;
   int expectedArgs = 0;
   std::vector<CodeLabel*> labels;
   
   while(*parseBuffer != NULL)
   {
      skipWhiteSpace(parseBuffer);

      char *tok = getToken(parseBuffer);
      int len = strlen(tok);
      switch(tok[len - 1])
      {
      case ':':
         {
            std::cout<<"Code label found: "<<tok<<std::endl;
            tok[len - 1] = NULL;    // Get rid of the :
            char *label = new char[len];
            strcpy(label, tok);
            labels.push_back(new CodeLabel(memLocation, label));
            break;
         }
      case ';':
         {
            std::cout<<"Comment found"<<std::endl;
            skipComment(parseBuffer);
            break;
         }
      case ',':
         {
            tok[len - 1] = NULL;    // Get rid of the ,
            std::cout<<"Comma found: "<<tok<<std::endl;
            if(strcmp(tok, ""))
               expectedArgs--;
            break;
         }
      default:
         {
            int numArgs = getNumArgs(tok);
            if(numArgs != -1)
            {
               std::cout<<"Instruction found: "<<tok<<" "<<numArgs<<std::endl;
               expectedArgs = numArgs;
            }
            else
            {
               std::cout<<"Argument found: "<<tok<<std::endl;
               expectedArgs--;
            }
         }
      }

      if(expectedArgs < 0)
      {
         std::cout<<"Error: too many arguments"<<std::endl;
         std::cin.get();
         return 1;
      }

      delete tok;
   }

   std::vector<CodeLabel*>::const_iterator iter = labels.begin();
   while(iter != labels.end())
   {
      std::cout<<(*iter)->getLabel()<<" ";
      std::cout<<(int)(*iter)->getMemLocation()<<std::endl;
      delete *iter;
      iter++;
   }

   delete[] buffer;

   std::cout<<"Code assembled"<<std::endl;
   std::cin.get();

   return 0;

}