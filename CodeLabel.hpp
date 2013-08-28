#ifndef CODELABEL_HPP
#define CODELABEL_HPP

class CodeLabel
{
   unsigned short memLocation;
   char *label;
public:
   CodeLabel(unsigned short memLoc, char *l);
   unsigned short getMemLocation();
   char *getLabel();
   ~CodeLabel();
};

#endif