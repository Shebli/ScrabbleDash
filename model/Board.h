#ifndef model_Board_h_INCLUDED
#define model_Board_h_INCLUDED

#include "Commons.h"
#include "model/Letter.h"
#include "model/Round.h"
#include "model/Slot.h"
#include <array>
#include <stdexcept>
#include <map>
#include <set>
#include <deque>
#include <memory>

namespace model {

class Placement;

class Board
{
public:
	class SlotIterator
	{
	public:
		SlotIterator(std::deque<Slot*>& allSlots, bool toEnd = false)
			: currentSlot(toEnd ? allSlots.end() : allSlots.begin()), endSlot(allSlots.end()) {}
		Slot& operator * () { return **currentSlot; }
		Slot* operator -> () { return *currentSlot; }
		SlotIterator& operator++() { ++currentSlot; return *this; }
		bool operator != (const SlotIterator& it_slot) const { return currentSlot != it_slot.currentSlot; }

	private:
		std::deque<Slot*>::iterator currentSlot;
		std::deque<Slot*>::iterator endSlot;
	};

	class ConstSlotIterator
	{
	public:
		ConstSlotIterator(const std::deque<Slot*>& allSlots, bool toEnd = false)
			: currentSlot(toEnd ? allSlots.end() : allSlots.begin()), endSlot(allSlots.end()) {}
		const Slot& operator * () const { return **currentSlot; }
		const Slot* operator -> () const { return *currentSlot; }
		ConstSlotIterator& operator++() { ++currentSlot; return *this; }
		bool operator != (ConstSlotIterator& it_slot) const { return currentSlot != it_slot.currentSlot; }

	private:
		std::deque<Slot*>::const_iterator currentSlot;
		std::deque<Slot*>::const_iterator endSlot;
	};

public:
	static const Index SIZE{Commons::MAX_LETTERS_PER_WORD};
	static bool isSlotCentral(Index row, Index col) { return SIZE%2==1 and row==((SIZE-1)/2) and col==((SIZE-1)/2); }
	static bool isSlotCentral(const Slot& slot) { return isSlotCentral(slot.row(), slot.col()); }

public:
	Board();
	int isize() const { return static_cast<int>(SIZE); }
		  Slot& slot(Index row, Index col);
	const Slot& slot(Index row, Index col) const;
	void checkSlotValid(Index row, Index col) const;
	SlotIterator begin() { return SlotIterator(allSlots); }
	ConstSlotIterator begin() const { return ConstSlotIterator(allSlots); }
	SlotIterator end() { return SlotIterator(allSlots, true); }
	ConstSlotIterator end() const { return ConstSlotIterator(allSlots, true); }

private:
	std::array< std::array<std::unique_ptr<Slot>, SIZE>, SIZE > slotArray;
	std::deque<Slot*> allSlots;
};

} // namespace model

#endif // model_Board_h_INCLUDED
