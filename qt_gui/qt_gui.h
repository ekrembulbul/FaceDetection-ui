#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_qt_gui.h"

class qt_gui : public QMainWindow
{
public:
	qt_gui(QWidget *parent = Q_NULLPTR);

private:
	Ui::qt_guiClass ui;
	Q_OBJECT
};
