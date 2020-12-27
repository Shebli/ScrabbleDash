#include "GLetter.h"
#include "ui_GLetter.h"

const QColor GLetter::BACKGROUND_COLOR{255, 249, 215};
const QColor GLetter::FOREGROUND_COLOR{40, 50, 40};
const QColor GLetter::VALUE_COLOR{50, 50, 50};

GLetter::GLetter(int letterCode, int letterValue, QWidget *parent) :
	QFrame(parent),
	ui(new Ui::GLetter)
{
	ui->setupUi(this);

	ui->letterGlyph->setText(QString(letterCode));
	ui->letterGlyph->setFont(QFont("Courier New"));
	ui->valueGlyph->setNum(letterValue);
}

GLetter::~GLetter()
{
	delete ui;
}
