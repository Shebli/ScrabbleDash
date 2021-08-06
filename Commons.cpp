#include "Commons.h"

const int Orientation::opposite[] = { 2, 3, 0, 1 };
const int Orientation::cmplment[] = { 1, 2, 3, 0 };
const Orientation Orientation::RIGHT(Orientation::ORKEY_RIGHT);
const Orientation Orientation::UP(Orientation::ORKEY_UP);
const Orientation Orientation::LEFT(Orientation::ORKEY_LEFT);
const Orientation Orientation::DOWN(Orientation::ORKEY_DOWN);

std::ostream&
operator<< (std::ostream& os, const Orientation& orientation)
{
	if (orientation == Orientation::RIGHT) os << "<RIGHT>";
	else if (orientation == Orientation::LEFT) os << "<LEFT>";
	else if (orientation == Orientation::UP) os << "<UP>";
	else if (orientation == Orientation::DOWN) os << "<DOWN>";
	return os;
}

std::ostream&
operator<< (std::ostream& os, SlotFactor slotfactor)
{
	if (slotfactor == REGULAR) os << "REGULAR";
	else if (slotfactor == LETTER_DOUBLE) os << "LETTER_DOUBLE";
	else if (slotfactor == WORD_DOUBLE) os << "WORD_DOUBLE";
	else if (slotfactor == WORD_TRIPLE) os << "WORD_TRIPLE";
	return os;
}

const std::string&
Exception::msg() const noexcept
{
	std::ostringstream oss;
	fillStream(oss);
	msg_ = oss.str();
	return msg_;
}

const char*
Exception::what() const noexcept
{
	return msg().c_str();
}
