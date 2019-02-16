#ifndef GBOARD_H
#define GBOARD_H

#include "Commons.h"
#include <QFrame>
#include <QColor>
#include <memory>

class GSlot;

namespace Ui {
class GBoard;
}

class GBoard : public QFrame
{
	Q_OBJECT

public:
	explicit GBoard(QWidget *parent = nullptr);
	~GBoard();
	GSlot* addSlot(int row, int col);
	const GSlot* gSlot(int row, int col) const;
		  GSlot* gSlot(int row, int col);

private:
	Ui::GBoard *ui;
};

#endif // GBOARD_H
