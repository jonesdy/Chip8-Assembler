#include "CodeLabel.hpp"

CodeLabel::CodeLabel()
{
   labelName = "unknown";
   location = 0;
}

CodeLabel::CodeLabel(std::string label, unsigned int loc)
{
   labelName = label;
   location = loc;
}

void CodeLabel::create(std::string label, unsigned int loc)
{
   labelName = label;
   location = loc;
}

std::string CodeLabel::getLabelName()
{
   return labelName;
}

unsigned int CodeLabel::getLocation()
{
   return location;
}
