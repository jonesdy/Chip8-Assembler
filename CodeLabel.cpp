#include "CodeLabel.hpp"

CodeLabel::CodeLabel(unsigned short memLoc, char *l)
{

   memLocation = memLoc;
   label = l;

}

unsigned short CodeLabel::getMemLocation()
{

   return memLocation;

}

char *CodeLabel::getLabel()
{

   return label;

}

CodeLabel::~CodeLabel()
{

   delete[] label;

}
