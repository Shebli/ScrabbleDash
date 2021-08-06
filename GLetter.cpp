#include "GLetter.h"
#include "ui_GLetter.h"

const QString GLetter::globalStyle(QString::fromUtf8(
									   "background-color: rgb(255, 249, 215);\n"
									   "color: rgb(127, 127, 127);"
									   ));
const QString GLetter::letterGlyphStyle(QString::fromUtf8(
										"font: 57 30pt \"Fira Sans Medium\";\n"
										"color: rgb(50, 50, 50);"
										));
const QString GLetter::valueGlyphStyle(QString::fromUtf8(
										"font: 25 8pt \"Fira Sans Light\";\n"
										"color: rgb(50, 50, 50);"
										));
const QString GLetter::nullGlyphStyle(QString::fromUtf8(
										"background-color: rgb(155, 149,115);\n"
										"color: rgb(255, 249, 215);"
										));

GLetter::GLetter(int letterCode, int letterValue, QWidget *parent) :
	QFrame(parent),
	ui(new Ui::GLetter),
	m_monLetter(nullptr)
{
	ui->setupUi(this);
	ui->letterGlyph->setText(QString(letterCode));
//	if (letterValue <= 0)
//	{
//		ui->letterGlyph->setStyleSheet(nullGlyphStyle);
//		ui->valueGlyph->setStyleSheet(nullGlyphStyle);
//		ui->valueGlyph->setText(".");
//	}
//	else
//	{
//		ui->letterGlyph->setStyleSheet(letterGlyphStyle);
//		ui->valueGlyph->setStyleSheet(valueGlyphStyle);
//		ui->valueGlyph->setText(QString::number(letterValue));
//	}
	ui->letterGlyph->setStyleSheet(letterGlyphStyle);
	ui->valueGlyph->setStyleSheet(valueGlyphStyle);
	ui->valueGlyph->setText(QString::number(letterValue));

}

GLetter::GLetter(MonitorPane::Letter& monLetter, int letterValue, QWidget *parent) :
	GLetter(monLetter.code, letterValue, parent)
{
	m_monLetter = &monLetter;
}

GLetter::~GLetter()
{
	delete ui;
}

int
GLetter::code() const
{
	return ui->letterGlyph->text()[0].unicode();
}

int
GLetter::value() const
{
	return ui->valueGlyph->text().toInt();
}
