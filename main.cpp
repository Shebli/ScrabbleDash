#include "Tests.h"
#include "MainDialog.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainDialog w;
	w.show();

	//Tests().run();

	return a.exec();
}
