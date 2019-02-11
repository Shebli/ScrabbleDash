#include "GBoard.h"
#include "ui_GBoard.h"

GBoard::GBoard(QWidget *parent) :
	QFrame(parent),
	ui(new Ui::GBoard)
{
	ui->setupUi(this);
}

GBoard::~GBoard()
{
	delete ui;
}
