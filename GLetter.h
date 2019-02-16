#ifndef GLETTER_H
#define GLETTER_H

#include <QFrame>
#include <QStaticText>

namespace Ui {
class GLetter;
}

class GLetter : public QFrame
{
	Q_OBJECT

public:
	explicit GLetter(int letterCode = ' ', int letterValue = 1, QWidget *parent = nullptr);
	~GLetter() override;
	auto code() const { return code_; }
	auto value() const { return value_; }

protected:
	void paintEvent(QPaintEvent *event) override;

private:
	Ui::GLetter *ui;
	QFont letterFont;
	QFont valueFont;
	QStaticText letterText;
	QStaticText valueText;
	QPointF letterPos;
	QPointF valuePos;
	int code_;
	int value_;
};

#endif // GLETTER_H
