#include "Placement.h"
#include "model/Slot.h"
namespace model
{

Placement::Placement(Slot& firstSlot, Orientation orientation)
	: firstSlot(firstSlot), orientation(orientation)
{
}

} // namespace model

std::ostream& operator<< (std::ostream& os, const model::Placement& placement)
{
	os << placement.firstSlot << placement.orientation;
	return os;
}
