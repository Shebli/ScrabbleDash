#ifndef GAMEDASH_H
#define GAMEDASH_H

#include <QFrame>
#include <memory>
#include <QVector>
#include <QLabel>
#include <QMultiMap>

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
	explicit GameDash(const QSize& screenSize, QWidget *parent = nullptr);
	~GameDash();

public slots:
	void slotClicked(int row, int col, bool isLeftButton);

private:
	Ui::GameDash *ui;
	int screenScale;
	QFont boardLetterFont;
	QFont paneLetterFont;
	QFont valueFont;
	std::unique_ptr<model::Board> up_board; ///< Holds the model::Board instance
	QVector< QVector<GSlot*> > gSlots;
	QMultiMap<int, QLabel*> availableLetters;
};

#endif // GAMEDASH_H
