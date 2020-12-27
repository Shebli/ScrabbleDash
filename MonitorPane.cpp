#include "MonitorPane.h"
#include "model/Letter.h"
#include "ui_MonitorPane.h"
#include <iostream>

const QLatin1String MonitorPane::STYLE_STRING
{
	"color: rgb(50, 50, 50);\n"
	"background-color: rgb(255, 249, 215);\n"
};

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
MonitorPane::getLetter(int row, int col)
{
	if (row >= ui->gridLayout->rowCount() or col >= ui->gridLayout->columnCount() or row < 0 or col < 0)
		return nullptr;

	if (ui->gridLayout->itemAtPosition(row, col) == nullptr)
		return nullptr;

	return dynamic_cast<QLabel*>(ui->gridLayout->itemAtPosition(row, col)->widget());
}

MonitorPane&
MonitorPane::setLetterFont(const QFont& letterFont_)
{
	letterFont = letterFont_;
	resetLetters();
	return *this;
}

QLabel*
MonitorPane::setLetter(int letterCode, int row, int col)
{
	QLabel* gLetter = getLetter(row, col);
	if (gLetter != nullptr)
		delete gLetter;

	// Allocating monitor letter
	gLetter = new QLabel(this);
	gLetter->setObjectName(uid(row, col));
	QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(gLetter->sizePolicy().hasHeightForWidth());
	gLetter->setSizePolicy(sizePolicy);
	gLetter->setStyleSheet(STYLE_STRING);
	gLetter->setFrameShape(QFrame::Panel);
	gLetter->setFrameShadow(QFrame::Raised);
	gLetter->setLineWidth(2);
	gLetter->setAlignment(Qt::AlignCenter);
	gLetter->setMargin(2);
	ui->gridLayout->addWidget(gLetter, row, col);

	// Setting value of monitor letter
	gLetter->setFont(letterFont); // @TODO: This call comes too early and it is not taken into account
	gLetter->setText(QString(letterCode));

	return gLetter;
}

void MonitorPane::resetLetters()
{
	std::cout << "Resetting monitor letters while font is: " << letterFont.toString().toStdString() << std::endl;
	int iRow{0}, iCol{0};
	for (const auto& letterBudget : model::Letter::budgetMap)
	{
		for (Index n=0; n < letterBudget.second; ++n)
		{
			setLetter(letterBudget.first, iRow, iCol++);
			if (iCol >= N_COLS)
			{
				iCol = 0;
				++iRow;
			}
		}
	}
}

void MonitorPane::resetLetter(int letterCode, int row, int col)
{
	setLetter(letterCode, row, col);
}

