#include "GameDash.h"
#include "ui_GameDash.h"
#include "model/Board.h"
#include "GSlot.h"
#include <iostream>

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
		connect(gSlot, SIGNAL(clicked(int, int)), this, SLOT(slotClicked(int, int)));
		gSlots[slot.irow()][slot.icol()] = gSlot;
		// Populate board with actual letters
		slotClicked(slot.irow(), slot.icol());
	}
}

GameDash::~GameDash()
{
	delete ui;
}

void
GameDash::slotClicked(int row, int col)
{
	std::cout << "Clicked on (" << row << "," << col << ")" << std::endl;
	gSlots[row][col]->placeLetter('W', 10);
}

