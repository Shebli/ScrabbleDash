#include "GLetter.h"
#include "model/Letter.h"
#include "ui_GLetter.h"
#include <QFont>
#include <QFontMetrics>
#include <QSizePolicy>
#include <QPainter>
#include <QPaintEvent>

using ::model::Letter;

const QColor GLetter::BACKGROUND_EMPTY (QColor::fromRgb(0, 0, 0, 0));
const QColor GLetter::BACKGROUND_PLACED (QColor::fromRgb(0, 127, 0, 255));
const QColor GLetter::FOREGROUND_LETTER(QColor::fromRgb(0, 0, 0, 255));
/*		"           color: rgb(255, 255, 255);\n"
		"            font: normal normal 24pt \"Fira Sans Medium\";"
*/
const QColor GLetter::FOREGROUND_VALUE(QColor::fromRgb(127, 127, 127, 255));
/*		"           color: rgb(127, 127, 127);\n"
		"            font: normal normal 30pt \"Fira Sans Light\";
*/

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

	auto height = (letterRect.height() + valueRect9.height()/3) * 1.1;
	auto width = (letterRect.width() + valueRect10.width()) * 1.2;
	if (height <= width)
		height = width;

	letterPos.setX(width*0.05);
	letterPos.setY(height*0.1);
	valuePos.setX(width-1.5*valueRect10.width());
	valuePos.setY(height-1.3*valueRect10.height());

	setMinimumSize(QSize(static_cast<int>(width), static_cast<int>(height)));
	setMaximumSize(minimumSize());
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
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

	if (code() == ' ')
	{
		setLineWidth(0);
		//painter.setBackground(BACKGROUND_EMPTY);
	}
	else
	{
		setLineWidth(1);
		//painter.setBackground(BACKGROUND_PLACED);
	}

	// Draw letter
	painter.setFont(letterFont);
	painter.setPen(FOREGROUND_LETTER);
	painter.drawStaticText(letterPos, letterText);

	// Draw value
	painter.setFont(valueFont);
	painter.setPen(FOREGROUND_VALUE);
	painter.drawStaticText(valuePos, valueText);
}
