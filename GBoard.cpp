#include "model/Board.h"
#include "GBoard.h"
#include "GLetter.h"
#include "ui_GBoard.h"

GBoard::GBoard(QWidget *parent) :
	QFrame(parent),
	ui(new Ui::GBoard)
{
	ui->setupUi(this);
	up_board.reset(new model::Board);
	for (auto row = 0; row < model::Board::SIZE; ++row)
	{
		for (auto col = 0; col < model::Board::SIZE; ++col)
		{
			ui->gridLayout->addWidget(new GLetter(this), row, col);
		}
	}
}

GBoard::~GBoard()
{
	delete ui;
}
