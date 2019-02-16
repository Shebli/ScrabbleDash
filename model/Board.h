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

public:
	Board();
	int isize() const { return static_cast<int>(SIZE); }
	Slot& slot(Index row, Index col);
	void checkSlot(Index row, Index col) const;
	const Slot& slot(Index row, Index col) const;

	bool placeLetter(Index row, Index col, char letterCode) { return placeLetter(slot(row, col), letterCode); }
	size_t placeString(const std::string& word);
	Board& nextRound(const Placement& placement);
	Board& nextRound(Index row, Index col, const Orientation& orientation);
	Board& nextRound();
		  Round& currentRound()       { return rounds.back(); }
	const Round& currentRound() const { return rounds.back(); }

		  Letter::Set& unusedLetters()       { return unusedLetters_; }
	const Letter::Set& unusedLetters() const { return unusedLetters_; }
	size_t roundCount() const { return rounds.size(); }
	SlotIterator begin() { return SlotIterator(allSlots); }
	ConstSlotIterator begin() const { return ConstSlotIterator(allSlots); }
	SlotIterator end() { return SlotIterator(allSlots, true); }
	ConstSlotIterator end() const { return ConstSlotIterator(allSlots, true); }

private:
	bool placeLetter(Slot& aSlot, char letterCode);

private:
	std::array< std::array<std::unique_ptr<Slot>, SIZE>, SIZE > slotArray;
	std::deque<Slot*> allSlots;
	Letter::Set unusedLetters_;
	std::deque<Round> rounds;
};

} // namespace model

#endif // model_Board_h_INCLUDED
