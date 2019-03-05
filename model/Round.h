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
	class EmptyException;

public:
	Round(size_t id);
	Round(size_t id, const Placement& placement_);
	void reset();
	void reset(const Placement& placement_);

	const Placement& placement() const;
		  Placement& placement();
	bool isEmpty() const { return !up_placement; }
	bool isFull() const { return isFull_; }
	void checkNotEmpty() const;
	size_t slotCount() const { return slotList.size(); }
	Orientation orientation() const { return placement().orientation; }

	Slot& currentSlot() { checkNotEmpty(); return *slotList.back(); }
	bool pushSlot();
	Slot& popSlot();
	auto begin() const { return slotList.begin(); }
	auto end() const { return slotList.end(); }

public:
	const size_t id;

private:
	std::unique_ptr<Placement> up_placement;
	std::deque<Slot*> slotList;
	bool isFull_;
};

class Round::EmptyException : Exception
{
public:
	explicit EmptyException(size_t numRound) : roundId(numRound) {}
	void fillStream(std::ostream& os) const noexcept override;

public:
	const size_t roundId;
};

} // namespace model

#endif // model_Round_h_INCLUDED
