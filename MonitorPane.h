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
	explicit MonitorPane(QWidget *parent = nullptr);
	~MonitorPane();

	MonitorPane& setLetterFont(const QFont& letterFont_) { letterFont = letterFont_; return *this; }
	QLabel* getLetter(int iLetter);
	QLabel* addLetter(int letterCode, int iLetter);

public:
	static QString uid(int iLetter) { return QStringLiteral("gLetter_")+QString::number(iLetter); }

private:
	Ui::MonitorPane *ui;
	QFont letterFont;
};

#endif // MONITORPANE_H
