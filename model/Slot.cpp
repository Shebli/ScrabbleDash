#include "Slot.h"

namespace model {


Slot::Slot(SlotFactor factor_, Index row_, Index col_)
	: row_(row_),
	  col_(col_),
	  factor_(factor_)
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
Slot::placeLetter(std::unique_ptr<Letter>&& nextLetter)
{
	if (isPlaced())
		throw AlreadyPlacedException(row(), col(), std::move(nextLetter));

	letter_ = std::move(nextLetter);
	std::cout << "Placed letter '" << letter().code() << "' at slot (" << row() << "," << col() << ")" << std::endl;
}

Slot*
Slot::neighbor(const Orientation& orientation)
{
	return neighbor_.at(orientation);
}

const Slot*
Slot::neighbor(const Orientation& orientation) const
{
	return neighbor_.at(orientation);
}

bool
Slot::isPlaced() const
{
	return letter_ and not letter().isNull();
}

bool
Slot::hasNeighbor(Orientation orientation) const
{
	return neighbor_.find(orientation) != neighbor_.end();
}

const Letter&
Slot::letter() const
{
	if (!letter_)
		throw Slot::EmptyException(row(), col()) ;

	return *letter_;
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
Slot::EmptyException::fillStream(std::ostream& os) const noexcept
{
	os << "Slot[" << row << "," << col << "] is empty";
}

void
Slot::AlreadyPlacedException::fillStream(std::ostream& os) const noexcept
{
	os << "Slot[" << row << "," << col << "] is not empty";
}

void
Slot::OutOfBoundsException::fillStream(std::ostream& os) const noexcept
{
	os << "Slot[" << row << "," << col << "] is outside board boundaries";
}

} // namespace model
