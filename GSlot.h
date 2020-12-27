#ifndef GSLOT_H
#define GSLOT_H

#include <QFrame>
#include <QString>

class GLetter;

namespace Ui {
class GSlot;
}

class GSlot : public QFrame
{
	Q_OBJECT

public:
	static QString uid(int row, int col);

public:
	explicit GSlot(int row, int col, QWidget *parent = nullptr);
	~GSlot() override;
	void placeLetter(int letterCode, int letterValue);
	const GLetter* gLetter() const;
		  GLetter* gLetter();
		  bool isEmpty() const;

		  QString uid() const { return uid(row, col); }

signals:
	void clicked(int row, int col, bool isLeftButton = true);

protected:
	void mousePressEvent(QMouseEvent* mouseEvent) override;

public:
	const int row;
	const int col;

private:
	Ui::GSlot *ui;
};

#endif // GSLOT_H
