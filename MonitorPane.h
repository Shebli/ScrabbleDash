#ifndef MONITORPANE_H
#define MONITORPANE_H

#include <QFrame>
#include <QLabel>
#include <QFont>

namespace Ui {
class MonitorPane;
}

class MonitorPane : public QFrame
{
	Q_OBJECT

public:
	static const int N_ROWS =  3;
	static const int N_COLS = 34;
	static const QLatin1String STYLE_STRING;

public:
	explicit MonitorPane(QWidget *parent = nullptr);
	~MonitorPane();
	QLabel* getLetter(int row, int col);
	QLabel* setLetter(int letterCode, int row, int col);
	MonitorPane& setLetterFont(const QFont& letterFont_);

public slots:
	void resetLetters();
	void resetLetter(int letterCode, int row, int col);

public:
	static QString uid(int row, int col)
	{
		return QStringLiteral("gLetter_")+QString::number(col + row*N_COLS);
	}

private:
	Ui::MonitorPane *ui;
	QFont letterFont;
};

#endif // MONITORPANE_H
