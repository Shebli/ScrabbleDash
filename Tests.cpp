#include "Tests.h"

using model::Board;
using model::Letter;
using model::Slot;
using model::Placement;

Tests::Tests(std::ostream& os) : os(os)
{
}

void
Tests::print(const std::string& msg) const
{
	os << msg << std::endl;
}

void
Tests::printLine() const
{
	os << std::string(80, '-') << std::endl;
}

void
Tests::printBoard() const
{
	os << "Round " << board.roundCount() << std::endl;
	for (Index row=0; row < Board::SIZE; ++row)
	{
		for (Index col=0; col < Board::SIZE; ++col)
		{
			auto& slot = board.slot(row, col);
			if (slot.isPlaced())
			{
				os << board.slot(row, col).letter().code();
			}
			else
			{
				switch(slot.factor())
				{
				case REGULAR:
					os << '.';
					break;

				case LETTER_DOUBLE:
					os << '+';
					break;

				case LETTER_TRIPLE:
					os << '#';
					break;

				case WORD_DOUBLE:
					os << '*';
					break;

				case WORD_TRIPLE:
					os << '$';
					break;
				}
			}
			os << ' ';
		}
		os << std::endl;
	}
}

void
Tests::printUnusedLetters() const
{
	os << "Remaining letters: " << board.unusedLetters().count() << std::endl;
	for (auto c='A'; c <= 'Z'; ++c)
	{
		os << c << ':' << board.unusedLetters().count(c) << ' ';
	}
	os << Letter::JOKER_CHAR << ':' << board.unusedLetters().count(Letter::JOKER_CHAR);
	os << std::endl;
}

void
Tests::run()
{
	printLine();
	printBoard();
	printUnusedLetters();

	printLine();
	board.placeString(7, 2, Orientation::RIGHT, "Extasiez");
	printBoard();
	printUnusedLetters();

	printLine();
	try { board.placeString(7, 15, Orientation::RIGHT, "TRACAS"); } catch (const Slot::OutOfBoundsException& e) { print(e.what()); }
	printBoard();
	printUnusedLetters();

	printLine();
	try { board.placeString(10, 10, Orientation::DOWN, "eminent"); } catch (const Slot::OutOfBoundsException& e) { print(e.what()); }
	printBoard();
	printUnusedLetters();

	printLine();
	try { board.placeString(6, 2, Orientation::DOWN, "MRCI"); } catch (const Slot::AlreadyPlacedException& e) { print(e.what()); }
	printBoard();
	printUnusedLetters();

	printLine();
	try { board.placeString(8, 0, Orientation::RIGHT, "DOMEZ"); } catch (const Letter::Set::NoMoreLetterException& e) { print(e.what()); }
	printBoard();
	printUnusedLetters();
}
