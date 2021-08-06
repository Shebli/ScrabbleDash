#include "Board.h"

namespace model {

/**
 * @fn bool isSlotCentral(Index row, Index col)
 * @brief Board::isSlotCentral Tests whether the specified slot position corresponds to the board's central slot.
 * @param row The slot row index.
 * @param col The slot column index.
 * @return true if the specified slot position is central, false otherwise.
 */

/**
 * @fn bool isSlotCentral(const Slot& slot)
 * @brief Board::isSlotCentral Tests whether the specified slot is the board's central slot.
 * @param slot the slot whose centrality is tested.
 * @return true if the specified slot is central, false otherwise.
 */

/**
 * @brief Board::Board Constructs a new board with all its 15x15 slots.
 * @details Each slot is initialized with the right bonus factor corresponding to a classic Scrabble board.
 */
Board::Board()
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

/**
 * @brief Board::checkSlot Makes sure that the specified row and column correspond to an actual slot on the board.
 * @param row Row number, the first row corresponding to number 0.
 * @param col Column number, the first row corresponding to number 0.
 * @throws OutOfBoundsException if the location specified by the row and column values is outside the board.
 * @throws UninitializedException if the slot location on the board had not been initilized with an actual Slot instance.
 */
void
Board::checkSlotValid(Index row, Index col) const
{
	if (row >= slotArray.size() or col >= slotArray[row].size())
		throw Slot::OutOfBoundsException(row, col);

	if (!slotArray[row][col])
		throw Slot::UninitializedException(row, col);
}

/**
 * @brief Board::slot Returns a reference to the slot occupying the specified position on the board.
 * @param row Row number of the slot.
 * @param col Column number of the slot.
 * @return Reference to the slot if it exists.
 * @throws OutOfBoundsException if the location specified by the row and column values is outside the board.
 * @throws UninitializedException if the slot location on the board had not been initilized with an actual Slot instance.
 */
Slot&
Board::slot(Index row, Index col)
{
	checkSlotValid(row, col);
	return *slotArray[row][col];
}

const Slot&
Board::slot(Index row, Index col) const
{
	checkSlotValid(row, col);
	return *slotArray[row][col];
}

} // namespace model
