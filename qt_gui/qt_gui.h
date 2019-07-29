#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_qt_gui.h"
#include "qtRecognizer.h"


class qt_gui : public QMainWindow
{
	Q_OBJECT

public:
	qt_gui(QWidget *parent = Q_NULLPTR);

private slots:
    void on_takePicture_clicked();
    void on_train_clicked();
    void on_mTrain_clicked();

    void on_predCam_clicked();

    void on_multiPredCam_clicked();

    void on_predImage_clicked();

private:
	Ui::qt_guiClass ui;

	recognizer r;
};
