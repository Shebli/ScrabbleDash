#ifndef MONITORPANE_H
#define MONITORPANE_H

#include <QFrame>
#include <QLabel>
#include <QFont>
#include <map>
#include <vector>

namespace Ui {
class MonitorPane;
}

class MonitorPane : public QFrame
{
	Q_OBJECT

public:
	class Letter;
	using SameCodeLetterSet = std::vector<Letter*>;

public:
	static const int N_ROWS = 3;
	static const int N_COLS = 34;
	static QString uid(int letterCode, size_t letterIndex);
	static QLatin1String unusedLetterStyle;
	static QLatin1String usedLetterStyle;
	static size_t unusedCount(const SameCodeLetterSet& letterSet);

public:
	explicit MonitorPane(QWidget *parent = nullptr);
	~MonitorPane();

	MonitorPane& setLetterFont(const QFont& letterFont_) { letterFont = letterFont_; return *this; }
	size_t unusedLettersCount() const;
	size_t unusedLettersCount(int letterCode) const;
	size_t allLettersCount(int letterCode) const;
	size_t allLettersCount() const;
	Letter* addLetter(int letterCode);
	Letter* popLetter(Letter* pLetter);
	Letter* pushLetter(Letter* pLetter);
	const Letter* getLetter(int letterCode, size_t letterIndex) const;
		  Letter* getLetter(int letterCode, size_t letterIndex);

private:
	Ui::MonitorPane *ui;
	QFont letterFont;
	std::map<int, SameCodeLetterSet> letters;
};

class MonitorPane::Letter : public QLabel
{
	Q_OBJECT

public:
	explicit Letter(int letterCode, unsigned index, QWidget *parent = nullptr);
	~Letter() {}
	QString uid() const { return MonitorPane::uid(code, index); }
	void mousePressEvent(QMouseEvent *mouseEvent) override;
	bool isPlaced() const { return m_isPlaced; }
	void setPlaced(bool isPlaced_ = true);

signals:
	void doPlace(MonitorPane::Letter*);

public:
	const int code;
	const unsigned index;

private:
	bool m_isPlaced;
};

#endif // MONITORPANE_H
