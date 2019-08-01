#pragma once

#include "ui_qt_gui.h"
#include "qtRecognizer.h"
#include "names.h"
#include "QImage"


enum states { INIT, NORMAL, TAKE_PICTURE, DISABLE, PRED, MULTI_PRED };


class qt_gui : public QMainWindow
{
	Q_OBJECT

public:
	qt_gui(QWidget *parent = Q_NULLPTR);
	~qt_gui();

public slots:
	void drawImage(cv::Mat frame);
	void guiEnabled();
	void slotStatePred();
	void slotStateMultiPred();

private slots:
    void on_trainStart_clicked();
    void on_mTrainStart_clicked();
    void on_takePictureStop_clicked();
    void on_takePictureStart_clicked();
    void on_predCamStart_clicked();
    void on_predCamStop_clicked();
    void on_multiPredCamStart_clicked();
    void on_multiPredCamStop_clicked();

private:
	Ui::qt_guiClass ui;
	void takeResultAndTrain(int dlgCode, int userCount, names &n);
	void setState(states state);
	void stateTrain();
	void stateNormal();
	void stateInit();
	void stateTakePicture();
	void statePred();
	void stateMultiPred();
	void setEnabledButtons(bool e);

	recognizer _r;
};
