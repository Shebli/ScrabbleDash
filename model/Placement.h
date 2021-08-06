#ifndef model_Placement_h_INCLUDED
#define model_Placement_h_INCLUDED

#include "Commons.h"
#include <ostream>

namespace model
{

class Slot;

class Placement
{
public:
	Placement(Slot& firstSlot, Orientation orientation);

public:
	Slot& firstSlot;
	Orientation orientation;
};

} // namespace model

std::ostream& operator<< (std::ostream& os, const model::Placement& placement);

#endif // model_Placement_h_INCLUDED
