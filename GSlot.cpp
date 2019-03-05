#include "GSlot.h"
#include "ui_GSlot.h"
#include "GLetter.h"
#include <iostream>
#include <QMouseEvent>

GSlot::GSlot(int row, int col, QWidget *parent)
	: QFrame(parent),
	  row(row),
	  col(col),
	  ui(new Ui::GSlot)
{
	ui->setupUi(this);
}

GSlot::~GSlot()
{
	delete ui;
}

void
GSlot::placeLetter(int letterCode, int letterValue)
{
	if (gLetter() != nullptr)
		delete gLetter();

	ui->verticalLayout->addWidget(new GLetter(letterCode, letterValue, this));
}

const GLetter*
GSlot::gLetter() const
{
	return findChild<GLetter*>("GLetter");
}

GLetter*
GSlot::gLetter()
{
	return findChild<GLetter*>("GLetter");
}

bool
GSlot::isEmpty() const
{
	return gLetter() == nullptr or gLetter()->code() == ' ';
}

void
GSlot::mousePressEvent(QMouseEvent *mouseEvent)
{
	switch (mouseEvent->button())
	{
	case Qt::RightButton:
		emit clicked(row, col, false);
		break;

	case Qt::LeftButton:
		emit clicked(row, col, true);
		break;

	default:
		/* Nothing */
		break;
	}
}
