#include "MonitorPane.h"
#include "ui_MonitorPane.h"

MonitorPane::MonitorPane(QWidget *parent) :
	QFrame(parent),
	ui(new Ui::MonitorPane),
	letterFont()
{
	ui->setupUi(this);
}

MonitorPane::~MonitorPane()
{
	delete ui;
}

QLabel*
MonitorPane::getLetter(int iLetter)
{
	return findChild<QLabel*>(uid(iLetter));
}

QLabel*
MonitorPane::addLetter(int letterCode, int iLetter)
{
	auto gLetter = new QLabel(this);
	gLetter->setObjectName(uid(iLetter));
	QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(gLetter->sizePolicy().hasHeightForWidth());
	gLetter->setSizePolicy(sizePolicy);

	gLetter->setStyleSheet(QLatin1String("font: 57 10pt \"Fira Sans Medium\";\n"
"color: rgb(50, 50, 50);\n"
"background-color: rgb(255, 249, 215);\n"
""));
	gLetter->setFrameShape(QFrame::Panel);
	gLetter->setFrameShadow(QFrame::Raised);
	gLetter->setLineWidth(2);
	gLetter->setAlignment(Qt::AlignCenter);
	gLetter->setMargin(2);

}
