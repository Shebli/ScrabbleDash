#include "Tests.h"
#include "GameDash.h"
#include <QApplication>
#include <QGuiApplication>
#include <QScreen>
#include <iostream>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	const auto& gui = *dynamic_cast<QGuiApplication*>(a.instance());
	GameDash w(gui.primaryScreen()->availableSize());
	w.setWindowState(w.windowState() ^ Qt::WindowFullScreen);
	w.show();

	//Tests().run();

	return a.exec();
}
