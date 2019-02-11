#ifndef GBOARD_H
#define GBOARD_H

#include <QFrame>

namespace Ui {
class GBoard;
}

class GBoard : public QFrame
{
	Q_OBJECT

public:
	explicit GBoard(QWidget *parent = nullptr);
	~GBoard();

private:
	Ui::GBoard *ui;
};

#endif // GBOARD_H
