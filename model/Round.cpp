#include "Round.h"
#include "model/Slot.h"

namespace model
{

Round::Round(size_t id) : id(id)
{
}

Round::Round(size_t id, const Placement& placement_) : id(id)
{
	up_placement.reset(new Placement(placement_));
	pushSlot(placement().firstSlot);
}

const Placement&
Round::placement() const
{
	if (isEmpty())
		throw EmptyRoundException(id);

	return *up_placement;
}

Placement&
Round::placement()
{
	if (isEmpty())
		throw EmptyRoundException(id);

	return *up_placement;
}

bool
Round::pushSlot(Slot& newSlot)
{
	for (auto p_slot : slotList)
	{
		if (&newSlot == p_slot)
			return false;
	}
	slotList.push_back(&newSlot);
	return true;
}

Slot*
Round::popSlot()
{
	if (slotCount() == 0)
		return nullptr;

	Slot* p_slot = slotList.back();
	slotList.pop_back();
	return p_slot;
}

void
Round::EmptyRoundException::fillStream(std::ostream& os) const noexcept
{
	os << "Round " << roundId << "is empty";
}

} // namespace model
