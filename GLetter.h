#ifndef GLETTER_H
#define GLETTER_H

#include "MonitorPane.h"
#include <QFrame>
#include <QColor>

namespace Ui {
class GLetter;
}

class GLetter : public QFrame
{
	Q_OBJECT

public:
	static const char CHAR_EMPTY = ' ';
	static const char CHAR_PLACEHOLDER = '.';
	static const QString globalStyle;
	static const QString letterGlyphStyle;
	static const QString valueGlyphStyle;
	static const QString nullGlyphStyle;

public:
	explicit GLetter(int letterCode = CHAR_EMPTY, int letterValue = 0, QWidget *parent = nullptr);
	explicit GLetter(MonitorPane::Letter& monLetter, int letterValue, QWidget *parent = nullptr);
	~GLetter() override;
	MonitorPane::Letter* monLetter() const { return m_monLetter; }
	int code() const;
	int value() const;


private:
	Ui::GLetter *ui;
	MonitorPane::Letter* m_monLetter;
};

#endif // GLETTER_H
