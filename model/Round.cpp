#include "Round.h"
#include "model/Slot.h"
#include "model/Board.h"

namespace model
{

/**
 * @brief Round::Round initializes a new empty round (no placement yet).
 * @param id The round number, first one being 0.
 * @param letterPool The letter pool from which letters will be retrieved to be place in the round.
 */
Round::Round(size_t id, Letter::Pool& letterPool)
	: id(id), m_isFull(false), m_letterPool(letterPool)
{
}

/**
 * @brief Round::Round initializes new round with round's first letter slot.
 * @param id The round number, first one being 0.
 * @param placement_ Theplacement of the first letter of the round.
 */
Round::Round(size_t id, Placement placement_, Letter::Pool& letterPool)
	: id(id), m_isFull(false), m_letterPool(letterPool)
{
	reset(placement_);
}

Round&
Round::reset()
{
	pm_placement.reset();
	return clear();
}

Round&
Round::reset(const Placement& placement_)
{
	if (placement_.firstSlot.isPlaced())
		throw Slot::AlreadyPlacedException(placement_.firstSlot.row(), placement_.firstSlot.col());

	pm_placement.reset(new Placement(placement_));
	return clear();
}

Round&
Round::clear()
{
	m_slotList.clear();
	m_isFull = false;
	if (not isEmpty() and not placement().firstSlot.isPlaced())
	{
		m_slotList.push_back(&placement().firstSlot);
	}
	return *this;
}

/**
 * @brief Return true if the word line of the round is legal.
 * @details Round is legal if the word line it defines touches an already placed slot or crosses the central slot.
 *          Also, an empty round is legal (corresponds to players who forgoe their turn).
 * @param asIs If true tests leglity of round as it is; if false tests potential legality of round i.e. even if round is
 *        not yet legal, it can become legal by adding more letters to it.
 */
bool
Round::isLegal(bool asIs) const
{
	bool legality = false;
	if (isEmpty())
	{
		legality = true;
	}
	else
	{
		const Slot* p_slot = &firstSlot();
		size_t slotCount = 1;
		do
		{
			if (Board::isSlotCentral(*p_slot))
			{
				legality = not asIs or p_slot->isPlaced();
			}
			else  // Try to see if you touch a placed neighbor
			{
				auto o = orientation();
				for (auto i=0; i < 4; ++i)
				{
					auto p_neighbor = p_slot->searchNeighbor(o);
					if (p_neighbor != nullptr and p_neighbor->isPlaced())
					{
						legality = not asIs or p_slot->isPlaced();
					}
					o = ~o;
				}
			}
			p_slot = p_slot->searchNeighbor(orientation());
			slotCount += 1;
		}
		while (not legality and p_slot != nullptr and slotCount <= Commons::MAX_LETTERS_PER_TURN);
	}
	return legality;
}

/**
 * @brief Returns reference to the round if the word line of the round is legal.
 * @details Round is legal if the word line it defines touches an already placed slot or crosses the central slot.
 *          Also, an empty round is legal (corresponds to players who forgoe their turn).
 * @param asIs If true tests leglity of round as it is; if false tests potential legality of round i.e. even if round is
 *        not yet legal, it can become legal by adding more letters to it.
 * @throws IllegalException if the round is deemed illegal.
 * @return Reference to the round if it is legal.
 */
Round&& Round::checkLegal(bool asIs)
{
	if (not isLegal(asIs))
		throw IllegalException(placement());

	return std::move(*this);
}

/**
 * @brief Round::checkNotEmpty Throws an exception if round is empty.
 * @return Reference to the round if it is not empty.
 * @throws EmptyException
 */
const Round&
Round::checkNotEmpty() const
{
	if (isEmpty())
		throw EmptyException(id);

	return *this;
}

/**
 * @brief Round::checkNotEmpty Throws an exception if round is empty.
 * @return Reference to the round if it is not empty.
 * @throws EmptyException
 */
Round&
Round::checkNotEmpty()
{
	if (isEmpty())
		throw EmptyException(id);

	return *this;
}

/**
 * @brief Round::checkNotFull Throws an exception if round is full.
 * @details Round is full if max number of letters per turn have been placed or there is no place left to add letters.
 * @return Reference to the next free slot if round is not full.
 * @throws FullException
 */
const Slot&
Round::checkNotFull() const
{
	if (slotCount() == Commons::MAX_LETTERS_PER_TURN)
		throw FullException(id);

	for (const Slot* p_slot = &lastSlot(); p_slot != nullptr; p_slot = p_slot->searchNeighbor(orientation()))
	{
		if (not p_slot->isPlaced())
			return *p_slot;
	}
	throw FullException(id);
}

/**
 * @brief Round::checkNotFull Throws an exception if round is full.
 * @details Round is full if max number of letters per turn have been placed or there is no place left to add letters.
 * @return Reference to the next free slot if round is not full.
 * @throws FullException
 */
Slot&
Round::checkNotFull()
{
	if (slotCount() == Commons::MAX_LETTERS_PER_TURN)
		throw FullException(id);

	for (Slot* p_slot = &lastSlot(); p_slot != nullptr; p_slot = p_slot->searchNeighbor(orientation()))
	{
		if (not p_slot->isPlaced())
			return *p_slot;
	}
	throw FullException(id);
}

/**
 * @brief Round::popSlot Removes the latest slot introduced in this round.
 * @details If last slot is first slot, it is not removed but if it is not empty, its letter is removed.
 * @return a pointer to the removed letter if removed slot was not empty, nullptr otherwise.
 * @throws Round::EmptyException if round has no slot yet.
 */
const Letter* Round::popSlot()
{
	checkNotEmpty();
	const Letter* poppedLetter = nullptr;

	// Remove letter of latest slot if not empty and if placed within this Round
	if (lastSlot().isPlaced())
	{
		poppedLetter = &m_letterPool.putBackLetter(lastSlot().removeLetter());
		std::cout << "Just put back letter '" << poppedLetter->charCode() << "' in pool." << std::endl;
	}

	// Pop slot if it is not the first
	if (slotCount() > 1)
	{
		std::cout << "Popping slot " << lastSlot() << std::endl;
		m_slotList.pop_back();
	}
	m_isFull = false;
	return poppedLetter;
}

/**
 * @brief Places letter specified by its code on current board round slot.
 * @param letterCode The letter to be placed.
 * @return Reference to the newly placed letter.
 * @throws Letter::Pool::NoMoreLetterException If there are no letter of specified code left in pool.
 * @throws Round::FullException if board's current round has no available slot left.
 * @throws Round::EmptyException if board's current round has no placement defined yet.
 */
const Letter&
Round::placeLetter(Letter::Code letterCode)
{
	Slot& nextSlot = checkNotFull();
	letterPool().checkAvailable(letterCode);
	if (&nextSlot != &lastSlot())
	{
		m_slotList.push_back(&nextSlot);
	}
	std::cout << "Slot count is " << slotCount() << " for round " << *this << std::endl;
	return nextSlot.placeLetter(letterPool().retrieveLetter(letterCode), *this);
}

/**
 * @brief Removes the last letter of the Round if it is not empty.
 * @details If the last slot was empty it is removed unless it is the first slot of the Round.
 * @return Pointer to the removed letter or nullptr if Round had no placed letter.
 */
const Letter*
Round::popLetter()
{
	const Letter* retVal = nullptr;
	while (retVal == nullptr and slotCount() > 0)
	{
		retVal = popSlot();
	}
	return retVal;
}

/**
 * @brief Round::placeString Places in this round all the letters it can from specified string.
 *	      Letters are placed up to the point where the board round limit is reached, in which case the placement stops.
 * @param word the string whose letters are placed.
 * @return the number of letters actually placed.
 * @throws Letter::Pool::NoMoreLetterException If there are no letter of specified code left in pool.
 * @throws Round::FullException if board's current round has no available slot left.
 * @throws Round::EmptyException if board's current round has no placement defined yet.
 */
size_t
Round::placeString(const std::string& word)
{
	size_t nPlaced = 0;
	try
	{
		for (const auto& letterCode : word)
		{
			placeLetter(letterCode);
		}
	}
	catch (const FullException&)
	{
	}
	return nPlaced;
}

void
Round::EmptyException::fillStream(std::ostream& os) const noexcept
{
	os << "Round " << roundId << " is empty";
}

void
Round::FullException::fillStream(std::ostream& os) const noexcept
{
	os << "Round " << roundId << " is full";
}

void
Round::IllegalException::fillStream(std::ostream& os) const noexcept
{
	os << "Round " << placement << " is illegal";
}

} // namespace model

std::ostream& operator<< (std::ostream& os, model::Round& round)
{
	os << "(" << round.id << ":" << round.placement() << ")";
	return os;
}
