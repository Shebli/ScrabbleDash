#include "model/Board.h"
#include "GBoard.h"
#include "GSlot.h"
#include "GLetter.h"
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

GSlot* GBoard::addSlot(int row, int col)
{
	auto gSlot = new GSlot(row, col, this);
	gSlot->setObjectName(gSlot->uid());
	ui->gridLayout->addWidget(gSlot, row, col);
	return gSlot;
}

const GSlot*
GBoard::gSlot(int row, int col) const
{
	return findChild<GSlot*>(GSlot::uid(row, col));
}

GSlot*
GBoard::gSlot(int row, int col)
{
	return findChild<GSlot*>(GSlot::uid(row, col));
}
