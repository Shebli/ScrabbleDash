#include "MonitorPane.h"
#include "ui_MonitorPane.h"
#include <QMouseEvent>
#include <iostream>

QLatin1String MonitorPane::unusedLetterStyle("font: 57 10pt \"Fira Sans Medium\";\n"
								   "color: rgb(50, 50, 50);\n"
								   "background-color: rgb(255, 249, 215);\n"
								   "");
QLatin1String MonitorPane::usedLetterStyle("font: 57 10pt \"Fira Sans Medium\";\n"
							   "color: rgb(200, 200, 200);\n"
							   "background-color: rgb(255, 249, 215);\n"
							   "");

/**
 * @brief MonitorPane::unusedCount counts the number of unused letters in set of letters sharing the same code.
 * @param letterSet The full set of letters sharing the same code.
 * @return The number of unused letters in the specified letter set.
 */
size_t
MonitorPane::unusedCount(const SameCodeLetterSet& letterSet)
{
	size_t count{0};
	for (const auto& pLetter : letterSet)
	{
		if (not pLetter->isPlaced())
		{
			count += 1;
		}
	}
	return count;
}

/**
 * @brief MonitorPane::MonitorPane constructs a MonitorPane widget. This widget displays all the letters available in
 *        the game. Already used letters are greyed out.
 * @param parent The parent Qt widget (typically the game global dashboard).
 */
MonitorPane::MonitorPane(QWidget *parent) :
	QFrame(parent),
	ui(new Ui::MonitorPane),
	letterFont()
{
	ui->setupUi(this);
}

/**
 * @brief MonitorPane::~MonitorPane destroys all children widgets.
 */
MonitorPane::~MonitorPane()
{
	delete ui;
}

/**
 * @brief MonitorPane::unusedLettersCount counts all the letters that have not yet been placed on the board.
 * @return The total number of unused letters.
 */
size_t
MonitorPane::unusedLettersCount() const
{
	size_t count{0};
	for (const auto& q : letters)
	{
		count += unusedCount(q.second);
	}
	return count;
}

/**
 * @brief MonitorPane::unusedLettersCount counts all the letters of s specific character that have not yet been placed
 *        on the board (e.g. all unused 'A' letters).
 * @param letterCode The specific character code of the letter set.
 * @return Total of all unused letters that share the same specified code.
 */
size_t
MonitorPane::unusedLettersCount(int letterCode) const
{
	auto it = letters.find(letterCode);
	return it != letters.end() ? unusedCount(it->second) : 0u;
}

/**
 * @brief MonitorPane::allLettersCount counts all the letters of the monitor pane sharing the same code (both used and
 *        unused), e.g. all 'A' letters of the pane.
 * @param letterCode The specific character code of the letter set.
 * @return Total number of all letters of the monitor pane that share the same specified code.
 */
size_t
MonitorPane::allLettersCount(int letterCode) const
{
	auto it = letters.find(letterCode);
	return it != letters.end() ? it->second.size() : 0u;
}

/**
 * @brief MonitorPane::allLettersCount counts all the letters of the monitor pane (both used and unused).
 * @return Total number of all letters of the monitor pane.
 */
size_t
MonitorPane::allLettersCount() const
{
	size_t count{0};
	for (const auto& q : letters)
	{
		count += q.second.size();
	}
	return count;
}

/**
 * @brief MonitorPane::addLetter adds a new letter to the monitor pane.
 * @param letterCode The character code of the new letter.
 * @return Pointer to the new letter instance just added to the monitor pane.
 */
MonitorPane::Letter*
MonitorPane::addLetter(int letterCode)
{
	auto index = allLettersCount(letterCode);
	auto letterPosition = allLettersCount();
	auto newLetter = new Letter(letterCode, index, this);
	letters[letterCode].push_back(newLetter);
	ui->gridLayout->addWidget(newLetter, letterPosition/N_COLS, letterPosition%N_COLS, 1, 1);
	return newLetter;
}

/**
 * @brief MonitorPane::popLetter Uses the specified letter out of the monitor pane if and only if it has not yet been
 *        used.
 * @param pLetter Pointer the letter to be used.
 * @return The letter pointer if it was not used before, nullptr otherwise.
 */
MonitorPane::Letter*
MonitorPane::popLetter(Letter* pMonLetter)
{
	if (pMonLetter != nullptr and not pMonLetter->isPlaced())
	{
		std::cout << "\tPopping '" << pMonLetter->uid().toStdString() << "' from monitor";
		pMonLetter->setPlaced(true);
		std::cout << " => remaining: " << unusedCount(letters[pMonLetter->code]) << std::endl;
		return pMonLetter;
	}
	return nullptr;
}

/**
 * @brief MonitorPane::pushLetter Puts the specified letter back into the monitor pane.
 * @param pLetter Pointer the letter to be used.
 * @return The letter pointer if it was not used before, nullptr otherwise.
 */
MonitorPane::Letter*
MonitorPane::pushLetter(Letter* pMonLetter)
{
	if (pMonLetter != nullptr and pMonLetter->isPlaced())
	{
		std::cout << "\tPushing back '" << pMonLetter->uid().toStdString() << "' into monitor";
		pMonLetter->setPlaced(false);
		std::cout << " => remaining: " << unusedCount(letters[pMonLetter->code]) << std::endl;
		return pMonLetter;
	}
	return nullptr;
}

/**
 * @brief MonitorPane::uid This static function computes a string id from a specific pair of letter code and index.
 *        For instance the uid of the 3rd instance the 'A' letter will be "A_2" (first instance index is '0').
 *        This funtion is used by the uid() method of class Letter.
 * @param letterCode The letter's character code.
 * @param letterIndex The letter's index.
 * @return The computed unique string ID.
 */
QString
MonitorPane::uid(int letterCode, size_t letterIndex)
{
	return QStringLiteral("letter_") + QString(letterCode) + "_" + QString::number(letterIndex);
}

/**
 * @brief MonitorPane::Letter::Letter Constructs a new monitor pane letter having the specified character code and index.
 * @param letterCode The letter's character code.
 * @param index The letter's index.
 * @param parent The parent widget of the letter: expected to be the monitor pane.
 */
MonitorPane::Letter::Letter(int letterCode, unsigned index, QWidget *parent)
	: QLabel(parent), code(letterCode), index(index), m_isPlaced(false)
{
	setText(QString(letterCode));
	setObjectName(uid());
	QSizePolicy sizePolicy_(QSizePolicy::Minimum, QSizePolicy::Minimum);
	sizePolicy_.setHorizontalStretch(0);
	sizePolicy_.setVerticalStretch(0);
	sizePolicy_.setHeightForWidth(sizePolicy().hasHeightForWidth());
	setSizePolicy(sizePolicy_);
	setFrameShape(QFrame::Panel);
	setFrameShadow(QFrame::Raised);
	setLineWidth(2);
	setAlignment(Qt::AlignCenter);
	setMargin(2);
	setStyleSheet(unusedLetterStyle);
}

/**
 * @brief MonitorPane::Letter::mousePressEvent issues a doUse signal if the letter has not yet been used.
 *        By left-clicking on the letter on the monitor pane, the user expresses his intent to place the letter on the
 *        board.
 * @param mouseEvent
 */
void
MonitorPane::Letter::mousePressEvent(QMouseEvent *mouseEvent)
{
	switch (mouseEvent->button())
	{
	case Qt::LeftButton:
		if (not isPlaced())
			emit doPlace(this);
		break;

	default:
		/* Nothing */
		break;
	}
}

/**
 * @brief MonitorPane::Letter::setPlaced Specifies whether the letter has been placed or not.
 * @param isPlaced_ Must be true if the letter has been placed.
 */
void
MonitorPane::Letter::setPlaced(bool isPlaced_)
{
	if (isPlaced_)
	{
		setStyleSheet(usedLetterStyle);
		m_isPlaced = true;
	}
	else
	{
		setStyleSheet(unusedLetterStyle);
		m_isPlaced = false;
	}
}
