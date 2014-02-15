#ifndef CODELABEL_HPP
#define CODELABEL_HPP

#include <string>

class CodeLabel
{
public:
   CodeLabel();
   CodeLabel(std::string label, unsigned int loc);
   void create(std::string label, unsigned int loc);
   std::string getLabelName();
   unsigned int getLocation();
   void setLabelName(std::string label);
   void setLocation(unsigned int loc);
private:
   std::string labelName;
   unsigned int location;
};

#endif
