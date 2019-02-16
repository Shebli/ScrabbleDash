#include "Board.h"

namespace model {

Board::Board() : unusedLetters_(Letter::budgetMap, Letter::valueMap)
{
	// Setting up board
	for (Index row=0; row < slotArray.size(); ++row)
	{
		for (Index col=0; col < slotArray[row].size(); ++col)
		{
			SlotFactor fact = REGULAR;
			Index r{row}, c{col};
			if (r > SIZE/2) r = SIZE - 1 - r;
			if (c > SIZE/2) c = SIZE - 1 - c;
			if (c > r) std::swap(r,c);
			switch(r-c)
			{
			case 0:
				switch(r)
				{
				case 0: fact = WORD_TRIPLE; break;
				case SIZE/2-2: fact = LETTER_TRIPLE; break;
				case SIZE/2-1: fact = LETTER_DOUBLE; break;
				default: fact = WORD_DOUBLE; break;
				}
				break;

			case SIZE/3-1:
				switch(c)
				{
				case 1: fact = LETTER_TRIPLE; break;
				case 2:
				case 3: fact = LETTER_DOUBLE; break;
				}
				break;

			case SIZE/4:
				if (c == 0) fact = LETTER_DOUBLE;
				break;

			case SIZE/2:
				if (c==0) fact = WORD_TRIPLE;
				break;
			}
			slotArray[row][col].reset(new Slot(fact, row, col));
			allSlots.push_back(&slot(row,col));
			if (row	> 0)
			{
				slotArray[row][col]->neighbor_[Orientation::UP] = slotArray[row-1][col].get();
				slotArray[row-1][col]->neighbor_[Orientation::DOWN] =	slotArray[row][col].get();
			}
			if (col > 0)
			{
				slotArray[row][col]->neighbor_[Orientation::LEFT] = slotArray[row][col-1].get();
				slotArray[row][col-1]->neighbor_[Orientation::RIGHT] = slotArray[row][col].get();
			}
		}
	}
}

void
Board::checkSlot(Index row, Index col) const
{
	if (row >= slotArray.size() or col >= slotArray[row].size())
		throw Slot::OutOfBoundsException(row, col);

	if (!slotArray[row][col])
		throw Slot::UninitializedException(row, col);
}

Slot&
Board::slot(Index row, Index col)
{
	checkSlot(row, col);
	return *slotArray[row][col];
}

const Slot&
Board::slot(Index row, Index col) const
{
	checkSlot(row, col);
	return *slotArray[row][col];
}

size_t
Board::placeString(const std::string& word)
{
	auto row = currentRound().lastSlot().row();
	auto col = currentRound().lastSlot().col();
	std::cout << "Placing '" << word << "' at (" << row << "," << col << ") "
			  << (currentRound().orientation() == Orientation::RIGHT ? "Horizontally" : "Vertically") << std::endl;

	size_t placed = 0;
	try
	{
		for (auto letterCode : word)
		{
			for (bool isPlaced = false; not isPlaced; currentRound().orientation().incr(row, col))
			{
				isPlaced = placeLetter(row, col, letterCode);
			}
			placed++;
		}
	}
	catch (const Slot::OutOfBoundsException&)
	{
	}
	return placed;
}

Board&
Board::nextRound(const Placement& placement)
{
	rounds.push_back(Round(rounds.size(), placement));
	return *this;
}

Board&
Board::nextRound(Index row, Index col, const Orientation& orientation)
{
	return nextRound(Placement(slot(row, col), orientation));
}

Board&
Board::nextRound()
{
	rounds.push_back(Round(rounds.size()));
	return *this;
}

bool
Board::placeLetter(Slot& aSlot, char letterCode)
{
	if (aSlot.isPlaced())
		return false;

	aSlot.placeLetter(unusedLetters().retrieveLetter(letterCode));
	return true;
}

} // namespace model
