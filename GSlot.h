#ifndef GSLOT_H
#define GSLOT_H

#include <QFrame>
#include <QColor>
#include <QString>
#include "Commons.h"

class GLetter;

namespace Ui {
class GSlot;
}

class GSlot : public QFrame
{
	Q_OBJECT

public:
	explicit GSlot(int row, int col, QWidget *parent = nullptr);
	~GSlot();
	void placeLetter(int letterCode, int letterValue);
	const GLetter* gLetter() const;
		  GLetter* gLetter();
		  bool isEmpty() const { return gLetter() == nullptr; }

		  QString uid() const { return uid(row, col); }

signals:
	void clicked(int row, int col);

protected:
	void mousePressEvent(QMouseEvent *) override;

public:
	static QString uid(int row, int col) { return QStringLiteral("GSlot-")+QString::number(row)+"-"+QString::number(col); }

public:
	const int row;
	const int col;

private:
	Ui::GSlot *ui;
};

#endif // GSLOT_H
