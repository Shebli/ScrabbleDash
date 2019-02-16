#include "GLetter.h"
#include "ui_GLetter.h"
#include <QFont>
#include <QFontMetrics>
#include <QSizePolicy>
#include <QPainter>

GLetter::GLetter(int letterCode, int letterValue, QWidget *parent) :
	QFrame(parent),
	ui(new Ui::GLetter),
	letterFont(QStringLiteral("Fira Sans Medium"), 24),
	valueFont(QStringLiteral("Fira Sans Light"), 8),
	letterText(QString(letterCode)),
	valueText(QString::number(letterValue)),
	code_(letterCode),
	value_(letterValue)
{
	ui->setupUi(this);

	letterText.prepare(QTransform(), letterFont);
	valueText.prepare(QTransform(), valueFont);

	QFontMetrics letterMetrics(letterFont);
	QFontMetrics valueMetrics(valueFont);
	auto letterRect = letterMetrics.boundingRect(QChar('W'));
	auto valueRect9 = valueMetrics.boundingRect(QStringLiteral("9"));
	auto valueRect10 = valueMetrics.boundingRect(QStringLiteral("10"));

//	auto height = letterRect.height() + valueRect9.height()/2;
//	auto width = letterRect.width() + valueRect10.width()/2;
	auto height = (letterRect.height() + valueRect9.height()/3) * 1.1;
	auto width = (letterRect.width() + valueRect10.width()) * 1.2;
	if (height <= width)
		height = width;

	letterPos.setX(width*0.05);
	letterPos.setY(height*0.1);
//	valuePos.setX(width-1.2*valueRect10.width());
//	valuePos.setY(height-1.1*valueRect10.height());
	valuePos.setX(width-1.5*valueRect10.width());
	valuePos.setY(height-1.3*valueRect10.height());

	setMinimumSize(QSize(static_cast<int>(width), static_cast<int>(height)));
	setMaximumSize(minimumSize());
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

//	ui->code->setText(QString(letterCode));
//	ui->code->setStyleSheet("font: bolder 24pt \"Fira Sans\";");
//	ui->value->setText(QString::number(letterValue));
//	ui->value->setStyleSheet("font: light 8pt \"Arial Narrow\"; color: rgb(124, 124, 124);");
}

GLetter::~GLetter()
{
	delete ui;
}

void
GLetter::paintEvent(QPaintEvent *event)
{
	// Drawing frame first
	QFrame::paintEvent(event);
	QPainter painter(this);
	painter.setPen(Qt::black);
	painter.setFont(letterFont);
	painter.drawStaticText(letterPos, letterText);
	painter.setFont(valueFont);
	painter.drawStaticText(valuePos, valueText);
}
