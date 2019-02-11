#include "Placement.h"
#include "model/Slot.h"
namespace model
{

Placement::Placement(Slot& firstSlot, Orientation orientation)
	: firstSlot(firstSlot), orientation(orientation)
{
}

} // namespace model
