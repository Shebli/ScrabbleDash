#include "GSlot.h"
#include "ui_GSlot.h"
#include "GLetter.h"
#include <iostream>

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

void
GSlot::mousePressEvent(QMouseEvent *)
{
	std::cout << "GSlot click" << std::endl;
	emit clicked(row, col);
}
