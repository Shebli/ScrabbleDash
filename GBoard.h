#ifndef GBOARD_H
#define GBOARD_H

#include <QFrame>
#include <memory>


namespace model {
class Board;
}

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
	std::unique_ptr<model::Board> up_board;
};

#endif // GBOARD_H
