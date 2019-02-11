#ifndef GLETTER_H
#define GLETTER_H

#include <QFrame>

namespace Ui {
class GLetter;
}

class GLetter : public QFrame
{
	Q_OBJECT

public:
	explicit GLetter(QWidget *parent = nullptr);
	~GLetter();

private:
	Ui::GLetter *ui;
};

#endif // GLETTER_H
