#ifndef GLETTER_H
#define GLETTER_H

#include <QFrame>
#include <QColor>

namespace Ui {
class GLetter;
}

class GLetter : public QFrame
{
	Q_OBJECT

public:
	static const QColor BACKGROUND_COLOR;
	static const QColor FOREGROUND_COLOR;
	static const QColor VALUE_COLOR;

public:
	explicit GLetter(int letterCode = ' ', int letterValue = 1, QWidget *parent = nullptr);
	~GLetter() override;

private:
	Ui::GLetter *ui;
};

#endif // GLETTER_H
