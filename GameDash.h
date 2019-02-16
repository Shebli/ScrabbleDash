#ifndef GAMEDASH_H
#define GAMEDASH_H

#include <QFrame>
#include <memory>
#include <QVector>

namespace model {
class Board;
}

class GSlot;

namespace Ui {
class GameDash;
}

class GameDash : public QFrame
{
	Q_OBJECT

public:
	explicit GameDash(QWidget *parent = nullptr);
	~GameDash();

public slots:
	void slotClicked(int row, int col);

private:
	Ui::GameDash *ui;
	std::unique_ptr<model::Board> up_board; ///< Holds the model::Board instance
	QVector< QVector<GSlot*> > gSlots;
};

#endif // GAMEDASH_H
