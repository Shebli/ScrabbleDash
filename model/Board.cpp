#include "Board.h"

namespace model {

/**
 * @brief Board::Board Constructs a new board with all its 15x15 slots.
 *        Each slot is initialized with the right bonus factor corresponding to a classic Scrabble board.
 */
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

/**
 * @brief Board::placeLetter Places specified letter on current slot in round.
 * @param aLetter The code of the letter to be placed.
 * @throws Slot::AlreadyPlacedException If current slot in round is already occupied by a letter.
 * @throws Round::EmptyException if board's current round has no placement defined yet.
 */
void Board::placeLetter(std::unique_ptr<Letter>&& aLetter)
{
	placeLetter(currentRound().currentSlot(), std::move(aLetter));
}

/**
 * @brief Board::placeString Creates a new round and places all the letters of a string at specified placement.
 *        The string is placed only if the placement slot is not already occupied.
 *        Every letter is placed only if the current slot is not yet occupied. If occupied the next slot is tried.
 *	      The letters are placed up to the point where the board limit is reached, in which case the placement stops.
 * @param placement the slot and orientation used to place the string letters.
 * @param word the string whose letters are placed.
 * @return the number of letters actually placed.
 * @throws Slot::AlreadyPlacedException If placement is already occupied by a letter.
 * @throws Slot::NoMoreLetterException if the word contains a letter that is no longer available in the game.
 */
size_t
Board::placeString(const Placement& placement, const std::string& word)
{
	size_t nPlaced = 0;
	nextRound(placement); // If placement already occupied exception is thrown.
	auto it_letterCode = word.begin();
	while
	(
	   not currentRound().isFull()
	   and it_letterCode != word.end()
	   and unusedLetters().count(*it_letterCode) > 0
	)
	{
		if (not currentRound().currentSlot().isPlaced())
		{
			placeLetter(unusedLetters().retrieveLetter(*it_letterCode));
			nPlaced++;
			++it_letterCode;
		}
		currentRound().pushSlot();
	}
	return nPlaced;
}

/**
 * @brief Board::placeString See overloaded placeString function.
 */
size_t
Board::placeString(Index row, Index col, Orientation orientation, const std::string& word)
{
	return placeString(Placement(slot(row, col), orientation), word);
}

/**
 * @brief Board::nextRound The current round is closed and a new one is opened.
 * @param placement The placement with which the new round begins.
 * @return A reference to the new round.
 * @throws Slot::AlreadyPlacedException If specified placement is already occupied by a letter.
 */
Round&
Board::nextRound(const Placement& placement)
{
	rounds.push_back(Round(rounds.size(), placement));
	return currentRound();
}

/**
 * @brief Board::nextRound The current round is closed and a new one is opened.
 * @param row The row number at which the new round begins.
 * @param col The column number at which the new round begins.
 * @return A reference to the new round.
 * @throws Slot::AlreadyPlacedException If specified placement is already occupied by a letter.
 */
Round&
Board::nextRound(Index row, Index col, const Orientation& orientation)
{
	return nextRound(Placement(slot(row, col), orientation));
}

/**
 * @brief Board::nextRound The current round is closed and a new one is opened.
 *        The new round is empty as it has no placement defined yet.
 * @return A reference to the new round.
 */
Round&
Board::nextRound()
{
	rounds.push_back(Round(rounds.size()));
	return currentRound();
}

/**
 * @brief Board::placeLetter Places specified letter at specified slot.
 * @param aSlot The slot where the letter is placed.
 * @param aLetter The letter to be placed.
 * @throws Slot::AlreadyPlacedException If specified slot is already occupied by a letter.
 */
void Board::placeLetter(Slot& aSlot, std::unique_ptr<Letter>&& aLetter)
{
		aSlot.placeLetter(std::move(aLetter), currentRound());
}

} // namespace model
