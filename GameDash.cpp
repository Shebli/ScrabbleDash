#include "GameDash.h"
#include "ui_GameDash.h"
#include "model/Board.h"
#include "model/Letter.h"
#include "GSlot.h"
#include <QRandomGenerator>
#include <iostream>

typedef model::Letter::Set::NoMoreLetterException NoMoreLetterException;

GameDash::GameDash(QWidget *parent) :
	QFrame(parent),
	ui(new Ui::GameDash),
	up_board(new model::Board)
{
	ui->setupUi(this);

	gSlots.resize(up_board->isize());
	for (auto& v : gSlots)
		v.resize(up_board->isize());

	for (auto& slot : *up_board)
	{
		auto gSlot = ui->boardPane->addSlot(slot.irow(), slot.icol());
		switch (slot.factor())
		{
		case REGULAR:
			gSlot->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
			break;

		case LETTER_DOUBLE:
			gSlot->setStyleSheet(QStringLiteral("background-color: rgb(155, 222, 255);"));
			break;

		case LETTER_TRIPLE:
			gSlot->setStyleSheet(QStringLiteral("background-color: rgb(51, 81, 255);"));
			break;

		case WORD_DOUBLE:
			gSlot->setStyleSheet(QStringLiteral("background-color: rgb(249, 181, 255);"));
			break;

		case WORD_TRIPLE:
			gSlot->setStyleSheet(QStringLiteral("background-color: rgb(218, 74, 74);"));
			break;
		}
		std::cout << "Connecting GSlot::clicked() to (" <<  gSlot->row << "," << gSlot->col << ")" << std::endl;
		connect(gSlot, SIGNAL(clicked(int, int, bool)), this, SLOT(slotClicked(int, int, bool)));
		gSlots[slot.irow()][slot.icol()] = gSlot;

		// Populate board with empty letters
		gSlots[slot.irow()][slot.icol()]->placeLetter(' ', 0);
	}
}

GameDash::~GameDash()
{
	delete ui;
}

void
GameDash::slotClicked(int row, int col, bool isLeftButton)
{
	auto urow = static_cast<Index>(row);
	auto ucol = static_cast<Index>(col);
	auto orientation = isLeftButton ? Orientation::DOWN : Orientation::RIGHT;
	std::cout << (orientation == Orientation::DOWN ? "V" : "H") << std::dec << "(" << urow << "," << ucol << ") : ";

	auto wordLength = 1+QRandomGenerator::global()->bounded(Commons::MAX_LETTERS_PER_TURN);
	if (wordLength > up_board->unusedLetters().count())
		wordLength = up_board->unusedLetters().count();

	if (wordLength > 0)
	{
		try
		{
			std::string newWord;
			std::vector<int> values;
			while (wordLength-- > 0)
			{
				auto iLetter = QRandomGenerator::global()->bounded(up_board->unusedLetters().count());
				auto randCode = up_board->unusedLetters()[iLetter].code();
				newWord.push_back(static_cast<char>(randCode));
				values.push_back(up_board->unusedLetters()[iLetter].value());
			}
			std::cout << "\"" << newWord << "\" -> '";
			up_board->placeString(urow, ucol, orientation, newWord);
			for (const auto& p_slot : up_board->currentRound())
			{
				if (p_slot->isPlaced() and p_slot->placementRound().id == up_board->currentRound().id)
				{
					gSlots[p_slot->irow()][p_slot->icol()]->placeLetter(p_slot->letter().charCode(), p_slot->letter().value());
					std::cout << p_slot->letter().charCode();
				}
			}
		}
		catch(model::Slot::AlreadyPlacedException& e)
		{
			up_board->unusedLetters().putBackLetter(std::move(e.refusedLetter));
		}
		std::cout << "' -> " << "Letters left: " << up_board->unusedLetters().count() << std::endl;
	}
}

