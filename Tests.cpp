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
	os << "Round " << game.roundCount() << std::endl;
	for (Index row=0; row < Board::SIZE; ++row)
	{
		for (Index col=0; col < Board::SIZE; ++col)
		{
			auto& slot = game.board().slot(row, col);
			if (slot.isPlaced())
			{
				os << game.board().slot(row, col).letter().code();
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
	os << "Remaining letters: " << game.letterPool().count() << std::endl;
	for (auto c='A'; c <= 'Z'; ++c)
	{
		os << c << ':' << game.letterPool().count(c) << ' ';
	}
	os << Letter::JOKER_CHAR << ':' << game.letterPool().count(Letter::JOKER_CHAR);
	os << std::endl;
}

void
Tests::run()
{
	printLine();
	printBoard();
	printUnusedLetters();

	printLine();
	game.pushNewRound(7, 2, Orientation::RIGHT).placeString("Extasiez");
	printBoard();
	printUnusedLetters();

	printLine();
	try { game.pushNewRound(7, 15, Orientation::RIGHT).placeString("TRACAS"); } catch (const Slot::OutOfBoundsException& e) { print(e.what()); }
	printBoard();
	printUnusedLetters();

	printLine();
	try { game.pushNewRound(10, 10, Orientation::DOWN).placeString("eminent"); } catch (const Slot::OutOfBoundsException& e) { print(e.what()); }
	printBoard();
	printUnusedLetters();

	printLine();
	try { game.pushNewRound(6, 2, Orientation::DOWN).placeString("MRCI"); } catch (const Slot::AlreadyPlacedException& e) { print(e.what()); }
	printBoard();
	printUnusedLetters();

	printLine();
	try { game.pushNewRound(8, 0, Orientation::RIGHT).placeString("DOMEZ"); } catch (const Letter::Pool::NoMoreLetterException& e) { print(e.what()); }
	printBoard();
	printUnusedLetters();
}
