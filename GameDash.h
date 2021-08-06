#ifndef GAMEDASH_H
#define GAMEDASH_H

#include <Commons.h>
#include "ui_GameDash.h"
#include <QFrame>
#include <QVector>
#include <QLabel>
#include <QMultiMap>
#include <memory>

namespace model {
class Game;
class Round;
}

class GSlot;

class GameDash : public QFrame
{
	Q_OBJECT

public:
	explicit GameDash(const QSize& screenSize, QWidget *parent = nullptr);
	~GameDash();
	static Index bounded(Index upperLimit);
	MonitorPane* monitorPane() { return ui->monitorPane; }
	const model::Game& game() const { return *m_game; }
		  model::Game& game()       { return *m_game; }
	bool isRoundOpen() const { return m_currentRound != nullptr; }
	const model::Round& currentRound() const;
		  model::Round& currentRound();

signals:
	void letterPlaced(int letterCode);
	void doBeginNewWord(Index row, Index col, Orientation orientation);
	void doEndNewWord();
	void doEraseLastLetter();

public slots:
	void slotClicked(int row, int col, bool isLeftButton);
	void beginNewWord(Index urow, Index ucol, Orientation orientation);
	void endNewWord();
	void placeLetterInNewWord(MonitorPane::Letter* pMonLetter);
	void removeLastLetterInNewWord();

private:
	void placeRandomString(int row, int col, bool isLeftButton);
	GSlot* currentGSlot();

private:
	Ui::GameDash *ui;
	int screenScale;
	QFont boardLetterFont;
	QFont paneLetterFont;
	QFont valueFont;
	QVector< QVector<GSlot*> > gSlots;
	QMultiMap<int, QLabel*> availableLetters;
	std::unique_ptr<model::Game> m_game; ///< Smart pointer is used to avoid including model header.
	model::Round* m_currentRound;
};

#endif // GAMEDASH_H
