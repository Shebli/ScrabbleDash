#include "Slot.h"
#include <iostream>

namespace model {


Slot::Slot(SlotFactor factor_, Index row_, Index col_)
	: row_(row_),
	  col_(col_),
	  factor_(factor_),
	  placementRound_(nullptr)
{
	neighbor_[ Orientation::RIGHT ] = nullptr;
	neighbor_[ Orientation::UP    ] = nullptr;
	neighbor_[ Orientation::LEFT  ] = nullptr;
	neighbor_[ Orientation::DOWN  ] = nullptr;
}

Slot::~Slot()
{
}

void
Slot::checkNotPlaced() const
{
	if (isPlaced())
		throw AlreadyPlacedException(row(), col());
}

void
Slot::checkIsPlaced() const
{
	if (not isPlaced())
		throw NotPlacedException(row(), col());
}

void
Slot::checkHasNeighbor(Orientation orientation) const
{
	if (not hasNeighbor(orientation))
		throw OutOfBoundsException(row(), col());
}

const Letter&
Slot::placeLetter(Letter::Token&& aLetter, const Round& aRound)
{
	try
	{
		checkNotPlaced();
	}
	catch (AlreadyPlacedException& e) // rethrow exception with the failed letter.
	{
		throw AlreadyPlacedException(e.row, e.col, std::move(aLetter));
	}

	m_letter = std::move(aLetter);
	placementRound_ = &aRound;
	return *m_letter;
}

Letter::Token
Slot::removeLetter()
{
	placementRound_ = nullptr;
	return std::move(m_letter);
}

Slot*
Slot::searchNeighbor(const Orientation& orientation)
{
	auto itNeighbor = neighbor_.find(orientation);
	return (itNeighbor == neighbor_.end() or itNeighbor->second == nullptr) ? nullptr : itNeighbor->second;
}

const Slot*
Slot::searchNeighbor(const Orientation& orientation) const
{
	auto itNeighbor = neighbor_.find(orientation);
	return (itNeighbor == neighbor_.end() or itNeighbor->second == nullptr) ? nullptr : itNeighbor->second;
}

Slot&
Slot::neighbor(const Orientation& orientation)
{
	return *neighbor_.at(orientation);
}

const Slot&
Slot::neighbor(const Orientation& orientation) const
{
	return *neighbor_.at(orientation);
}

bool
Slot::isPlaced() const
{
	std::cout << "---" << irow() << "," << icol() << ":";
	if (!m_letter)
		std::cout << "{NULL}";
	else std::cout << letter();
	std::cout << std::endl;
	return m_letter.get() != nullptr and not letter().isNull();
}

const Letter&
Slot::letter() const
{
	if (!m_letter)
		throw Slot::NotPlacedException(row(), col()) ;

	return *m_letter;
}

Index
Slot::row() const
{
	return row_;
}

Index Slot::col() const
{
	return col_;
}

int
Slot::letterFactor() const
{
	return factor() & 0x0000FFFFu & factor_;
}

int
Slot::wordFactor() const
{
	return factor() >> 16;
}

SlotFactor
Slot::factor() const
{
	return factor_;
}

Slot::Exception::~Exception()
{
}

void
Slot::UninitializedException::fillStream(std::ostream& os) const noexcept
{
	os << "Slot[" << row << "," << col << "] is not initialized";
}

void
Slot::NotPlacedException::fillStream(std::ostream& os) const noexcept
{
	os << "Slot[" << row << "," << col << "] is empty";
}

void
Slot::AlreadyPlacedException::fillStream(std::ostream& os) const noexcept
{
	os << "Slot[" << row << "," << col << "] is not empty (contains letter " << *refusedLetter << ")" << std::endl;
}

void
Slot::OutOfBoundsException::fillStream(std::ostream& os) const noexcept
{
	os << "Slot[" << row << "," << col << "] is outside board boundaries";
}

} // namespace model

std::ostream& operator<< (std::ostream& os, const model::Slot& aSlot)
{
	os << "[" << aSlot.irow() << "," << aSlot.icol() << "," << aSlot.factor();
	if (aSlot.isPlaced()) os << aSlot.letter();
	else os << model::Letter();
	os << "]";
	return os;
}
