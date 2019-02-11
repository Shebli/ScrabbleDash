#ifndef model_Placement_h_INCLUDED
#define model_Placement_h_INCLUDED

#include "Commons.h"

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

#endif // model_Placement_h_INCLUDED
