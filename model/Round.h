#ifndef model_Round_h_INCLUDED
#define model_Round_h_INCLUDED

#include "model/Placement.h"
#include "Commons.h"
#include <deque>
#include <memory>

namespace model
{

class Slot;

class Round
{
public:
	class EmptyRoundException;

public:
	Round(size_t id);
	Round(size_t id, const Placement& placement_);

	const Placement& placement() const;
		  Placement& placement();
		  bool isEmpty() const { return !up_placement; }
	size_t slotCount() const { return slots.size(); }
	const Slot& firstSlot() const { return placement().firstSlot; }
	const Slot& lastSlot() const { return *slots.back(); }
	Orientation orientation() const { return placement().orientation; }

	bool pushSlot(Slot& newSlot);
	Slot* popSlot();
	auto begin() const { return slots.end(); }
	auto end() const { return slots.end(); }

public:
	const size_t id;

private:
	std::unique_ptr<Placement> up_placement;
	std::deque<Slot*> slots;
};

class Round::EmptyRoundException : Exception
{
public:
	explicit EmptyRoundException(size_t numRound) : roundId(numRound) {}
	void fillStream(std::ostream& os) const noexcept override;

public:
	const size_t roundId;
};

} // namespace model

#endif // model_Round_h_INCLUDED
