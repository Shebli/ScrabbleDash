#include "GSlot.h"
#include "ui_GSlot.h"
#include "GLetter.h"
#include <QMouseEvent>
#include <sstream>
#include <iomanip>

QString
GSlot::uid(int row, int col)
{
	std::ostringstream oss;
	oss << "GSlot_" << std::dec << std::setfill('0') << std::setw(2) << row << "-" << std::setw(2) << col;
	return QString::fromStdString(oss.str());
}

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
	return gLetter() == nullptr;
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
