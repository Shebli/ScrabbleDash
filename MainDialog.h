#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <memory>

class GameControl;

namespace Ui {
class MainDialog;
}

class MainDialog : public QDialog
{
	Q_OBJECT

public:
	explicit MainDialog(QWidget *parent = nullptr);
	~MainDialog();

private:
	Ui::MainDialog *ui;

};

#endif // MAINDIALOG_H
