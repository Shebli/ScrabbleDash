#include "GLetter.h"
#include "ui_GLetter.h"

GLetter::GLetter(QWidget *parent) :
	QFrame(parent),
	ui(new Ui::GLetter)
{
	ui->setupUi(this);
}

GLetter::~GLetter()
{
	delete ui;
}
