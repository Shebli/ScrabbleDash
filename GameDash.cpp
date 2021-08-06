#include "GameDash.h"
#include "model/Game.h"
#include "GSlot.h"
#include "GLetter.h"
#include "GBoard.h"
#include "GStencil.h"
#include <QRandomGenerator>
#include <iostream>
#include <algorithm>

using NoMoreLetterException = model::Letter::Pool::NoMoreLetterException;

GameDash::GameDash(const QSize& screenSize, QWidget *parent)
	: QFrame(parent),
	  ui(new Ui::GameDash),
	  screenScale(std::min(screenSize.width(), screenSize.height())),
	  boardLetterFont(QStringLiteral("Fira Sans Medium"), screenScale * 24 / 1024),
	  paneLetterFont(QStringLiteral("Fira Sans Medium"), screenScale * 10 / 1024),
	  valueFont(QStringLiteral("Fira Sans Light"), screenScale * 8 / 1024),
	  m_game(new model::Game()),
	  m_currentRound(nullptr)
{
	ui->setupUi(this);
	monitorPane()->setLetterFont(paneLetterFont);

	gSlots.resize(game().board().isize());
	for (auto& v : gSlots)
		v.resize(game().board().isize());

	// Populate board with all its letter slots
	for (const auto& slot : game().board())
	{
		auto gSlot = ui->boardControl->gBoard().addSlot(slot.irow(), slot.icol());
		switch (slot.factor())
		{
		case REGULAR:
			// Keep default background color
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
		// Connecting every board slot click to GameDash::slotClicked
		connect(gSlot, SIGNAL(clicked(int, int, bool)), this, SLOT(slotClicked(int, int, bool)));

		// Keeping reference to every graphical board slot in GameDash
		gSlots[slot.irow()][slot.icol()] = gSlot;

		// gSlots[slot.irow()][slot.icol()]->placeLetter('W', 10);
	}

	// Populate board monitor with all letters
	auto nLetters = game().letterPool().count();
	for (Index i=0; i < nLetters; ++i)
	{
		auto gMonLetter = monitorPane()->addLetter(game().letterPool()[i].code());
		connect(gMonLetter, SIGNAL(doPlace(MonitorPane::Letter*)), this, SLOT(placeLetterInNewWord(MonitorPane::Letter*)));
	}

	// Connect monitor pane events
	connect(this, SIGNAL(doBeginNewWord(Index, Index, Orientation)), this, SLOT(beginNewWord(Index, Index, Orientation)));
	connect(this, SIGNAL(doEndNewWord()), this, SLOT(endNewWord()));
	connect(this, SIGNAL(doEraseLastLetter()), this, SLOT(removeLastLetterInNewWord()));

	// Connect application Quit button.
	connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(close()));
}

GameDash::~GameDash()
{
	delete ui;
}

Index GameDash::bounded(Index upperLimit)
{
	return QRandomGenerator::global()->bounded(static_cast<quint32>(upperLimit));
}

model::Round&
GameDash::currentRound()
{
	if (not isRoundOpen())
		throw model::Game::NoOpenRoundException();

	return *m_currentRound;
}

const model::Round&
GameDash::currentRound() const
{
	if (not isRoundOpen())
		throw model::Game::NoOpenRoundException();

	return *m_currentRound;
}

void
GameDash::slotClicked(int row, int col, bool isLeftButton)
{
	auto orientation = isLeftButton ? Orientation::DOWN : Orientation::RIGHT;

	// A there a new Round has already been opened
	if (isRoundOpen())
	{
		if (isLeftButton) // Close the new placement if left button is clicked
		{
			emit doEndNewWord();
		}
		else // Erase last inserted letter (if any) if right mouse button is clicked
		{
			// If new word is empty, then cancel the new word
			if (not currentRound().firstSlot().isPlaced())
			{
				emit doEndNewWord();
			}
			else
			{
				emit doEraseLastLetter();
			}
		}
	}
	else // A new word begins to be placed only if the clicked slot is empty
	{
		emit doBeginNewWord(row, col, orientation);
	}
}

void
GameDash::beginNewWord(Index urow, Index ucol, Orientation orientation)
{
	try
	{
		// New round is opened only if selected slot is empty (not already placed)
		if (not game().board().slot(urow, ucol).isPlaced())
		{
			// Open a new round on the board model
			m_currentRound = &m_game->pushNewRound(urow, ucol, orientation);
			std::cout << "Created round " << *m_currentRound << std::endl;

			// Display graphically all the slots that are available for this new word
			auto pSlot = &currentRound().firstSlot();
			size_t slotCount = 0;
			do
			{
				if (not pSlot->isPlaced())
				{
					gSlots[pSlot->row()][pSlot->col()]->placeLetter(); // Place placeholder (fake) letter
					slotCount += 1;
				}
				pSlot = pSlot->hasNeighbor(orientation) ? &pSlot->neighbor(orientation) : nullptr;
			}
			while (pSlot != nullptr and slotCount < Commons::MAX_LETTERS_PER_TURN);
		}
	}
	catch (const model::Slot::AlreadyPlacedException& e)
	{
		std::cerr << e.msg() << std::endl;
	}
	catch (const model::Round::IllegalException& e)
	{
		std::cerr << e.msg() << std::endl;
	}
}

void
GameDash::endNewWord()
{
	try
	{
		// Remove graphically all the slots that have not been occupied by this new word
		auto pSlot = &currentRound().checkLegal(true).firstSlot(); // check if round is legal as it is
		std::cout << "End Word: processing slot " << *pSlot << std::endl;
		do
		{
			if (not pSlot->isPlaced())
			{
				std::cout << "\tCleaning up slot " << *pSlot << std::endl;
				gSlots[pSlot->row()][pSlot->col()]->removeLetter();
			}
			else std::cout << "\tLeaving slot " << *pSlot << " as it is" << std::endl;
			pSlot = pSlot->hasNeighbor(currentRound().orientation()) ?
						&pSlot->neighbor(currentRound().orientation()) : nullptr;
		}
		while (pSlot != nullptr);

		// The new word is sealed as the next round only if the round is not empty
		if (not currentRound().firstSlot().isPlaced())
		{
			std::cout << "Abort empty new word in round " << currentRound() << std::endl;
			game().popLastRound();
		}
		else std::cout << "Seal round " << currentRound() << std::endl;
		m_currentRound = nullptr;
	}
	catch (const model::Round::IllegalException& e)
	{
		std::cerr << e.msg() << std::endl;
	}
}

void
GameDash::placeLetterInNewWord(MonitorPane::Letter* pMonLetter)
{
	// The new letter is added to the current slot (if a round has been opened)
	if (isRoundOpen())
	{
		try
		{
			// Retrieve and place letter if available in model
			const model::Letter& placedLetter = currentRound().placeLetter(pMonLetter->code);
			std::cout << "Placed letter " << placedLetter << " in round " << currentRound() << std::endl;

			// Use letter caracteristics for on-board and monitor display
			currentGSlot()->placeLetter(*pMonLetter, placedLetter.value());
			monitorPane()->popLetter(pMonLetter);
		}
		catch (const model::Round::FullException& e)
		{
			std::cerr << e.msg() << std::endl;
		}
	}
}

void
GameDash::removeLastLetterInNewWord()
{
	// Last letter inserted into new word is deleted
	// Un-place letter from current slot
	if (isRoundOpen())
	{
		// Remember current graphical slot before current slot is popped
		auto p_gSlot = currentGSlot();
		// Remove letter and put it back in letter pool
		if (currentRound().popLetter() != nullptr)
		{
			// Remove letter graphically
			monitorPane()->pushLetter(p_gSlot->gLetter()->monLetter());
			p_gSlot->placeLetter();
		}
	}
}

/**
 * @brief GameDash::placeRandomString
 *        Places a random string made from remaining letters at the slot that has been clicked provided that it is empty.
 * @param row The row number of the string's first letter.
 * @param col The column number of the string's first letter.
 * @param isLeftButton If true (left mouse button has been clicked) the placed word is vertical.
 */
void
GameDash::placeRandomString(int row, int col, bool isLeftButton)
{
	auto urow = static_cast<Index>(row);
	auto ucol = static_cast<Index>(col);
	auto orientation = isLeftButton ? Orientation::DOWN : Orientation::RIGHT;
	std::cout << (orientation == Orientation::DOWN ? "V" : "H") << std::dec << "(" << urow << "," << ucol << ") : ";

	auto wordLength = 1+bounded(Commons::MAX_LETTERS_PER_TURN);
	if (wordLength > game().letterPool().count())
		wordLength = game().letterPool().count();

	if (wordLength > 0)
	{
		try
		{
			std::string newWord;
			std::vector<int> values;
			while (wordLength-- > 0)
			{
				auto iLetter = bounded(game().letterPool().count());
				auto randCode = game().letterPool()[iLetter].code();
				newWord.push_back(static_cast<char>(randCode));
				values.push_back(game().letterPool()[iLetter].value());
			}
			std::cout << "\"" << newWord << "\" -> \n";
			beginNewWord(urow, ucol, orientation);
			if (m_currentRound != nullptr)
			{
				currentRound().placeString(newWord);
				for (const auto& mod_slot : *m_currentRound)
				{
					if (mod_slot->isPlaced() and mod_slot->placementRound().id == currentRound().id)
					{
						gSlots[mod_slot->irow()][mod_slot->icol()]->placeLetter (
									mod_slot->letter().code(), mod_slot->letter().value()
									);
						emit letterPlaced(mod_slot->letter().code());
					}
				}
			}
		}
		catch(model::Slot::AlreadyPlacedException& e)
		{
			if (m_currentRound != nullptr)
			{
				currentRound().letterPool().putBackLetter(std::move(e.refusedLetter));
			}
		}
		std::cout << "' -> " << "Letters left: " << game().letterPool().count() << std::endl;
	}
}

/**
 * @brief GameDash::currentGSlot
 * @return Point to the current GSlot of the board.
 */
GSlot*
GameDash::currentGSlot()
{
	return gSlots[currentRound().lastSlot().irow()][currentRound().lastSlot().icol()];
}
