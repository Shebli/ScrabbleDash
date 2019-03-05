#include "Round.h"
#include "model/Slot.h"

namespace model
{

/**
 * @brief Round::Round initializes a new empty round (no placement yet).
 * @param id The round number, first one being 0.
 */
Round::Round(size_t id) : id(id), isFull_(false)
{
}

/**
 * @brief Round::Round initializes new round with round's first letter slot.
 * @param id The round number, first one being 0.
 * @param placement_ Theplacement of the first letter of the round.
 */
Round::Round(size_t id, const Placement& placement_) : id(id), isFull_(false)
{
	reset(placement_);
}

void
Round::reset()
{
	up_placement.reset();
	slotList.clear();
	isFull_ = false;
}

void
Round::reset(const Placement& placement_)
{
	if (placement_.firstSlot.isPlaced())
		throw Slot::AlreadyPlacedException(placement_.firstSlot.row(), placement_.firstSlot.col());

	up_placement.reset(new Placement(placement_));
	slotList.clear();
	slotList.push_back(&placement().firstSlot);
	isFull_ = false;
}

/**
 * @brief Round::placement
 * @return const reference to this round's placement.
 */
const Placement&
Round::placement() const
{
	if (isEmpty())
		throw EmptyException(id);

	return *up_placement;
}

/**
 * @brief Round::placement
 * @return non-const reference to this round's placement.
 * @throws Round::EmptyException if board's current round has no placement defined yet.
 */
Placement&
Round::placement()
{
	checkNotEmpty();
	return *up_placement;
}

/**
 * @brief Round::assertNotEmpty Throws an exception if round is empty.
 * @throws EmptyException
 */
void
Round::checkNotEmpty() const
{
	if (isEmpty())
		throw EmptyException(id);
}

/**
 * @brief Round::pushSlot Adds next neighbor slot to round.
 * @return true if next neighbor has been added, false if there is no next neighbor (board border reached).
 * @throws Round::EmptyException if round has no placement defined yet.
 * @throws Slot::NotPlacedException if the neighbor slot that is being pushed in this round is empty (has no placed letter).
 */
bool Round::pushSlot()
{
	currentSlot().checkIsPlaced();
	if (currentSlot().hasNeighbor(placement().orientation))
	{
		slotList.push_back(&currentSlot().neighbor(orientation()));
	}
	else
	{
		isFull_ = true;
	}
	return not isFull();
}

/**
 * @brief Round::popSlot Removes the latest slot introduced in this round.
 * @return a reference to the removed slot.
 * @throws Round::EmptyException if round has no slot yet.
 */
Slot& Round::popSlot()
{
	checkNotEmpty();
	Slot* p_slot = slotList.back();
	slotList.pop_back();
	isFull_ = false;
	return *p_slot;
}

void
Round::EmptyException::fillStream(std::ostream& os) const noexcept
{
	os << "Round " << roundId << "is empty";
}

} // namespace model
