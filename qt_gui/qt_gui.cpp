#include "qt_gui.h"
#include "names.h"
#include <map>
#include <string>
#include <qdebug.h>
#include <Windows.h>


qt_gui::qt_gui(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setState(INIT);
	ui.videoLabel->setPixmap(QPixmap::fromImage(QImage("icon/camera.png")));
	connect(&_r, &recognizer::readyImage, this, &qt_gui::drawImage);
	connect(&_r, &recognizer::signalGuiEnable, this, &qt_gui::guiEnabled);
	connect(&_r, &recognizer::signalStatePred, this, &qt_gui::slotStatePred);
	connect(&_r, &recognizer::signalStateMultiPred, this, &qt_gui::slotStateMultiPred);
}


qt_gui::~qt_gui()
{
	_r.stopTakePicture();
	_r.stopPredictFromCam();
	_r.stopMultiPredictFromCam();
	_r.stopTrain();
}


void qt_gui::on_takePictureStart_clicked()
{
	setState(TAKE_PICTURE);
	int userId = ui.userIdInput->value();
	_r.startTakePicture(userId);
}


void qt_gui::on_takePictureStop_clicked()
{
	_r.stopTakePicture();
	setState(NORMAL);
}


void qt_gui::drawImage(cv::Mat frame)
{
	qDebug() << __FUNCTION__" :" << GetCurrentThreadId();

	if (!frame.empty())
	{
		cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
		QImage image = QImage((uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
		ui.videoLabel->setPixmap(QPixmap::fromImage(image));
	}
	else ui.videoLabel->setPixmap(QPixmap::fromImage(QImage("icon/camera.png")));
}


void qt_gui::on_trainStart_clicked()
{
	setState(DISABLE);
	int userId = ui.userIdInputTrain->value();
	_r.startTrain(userId);
}


void qt_gui::guiEnabled()
{
	setState(NORMAL);
}


void qt_gui::slotStatePred()
{
	setState(NORMAL);
	setState(PRED);
}


void qt_gui::slotStateMultiPred()
{
	setState(NORMAL);
	setState(MULTI_PRED);
}

void qt_gui::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) _r.setSaveFace(true);
}

void qt_gui::mouseReleaseEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) _r.setSaveFace(false);
}


void qt_gui::on_mTrainStart_clicked()
{
	int userCount = ui.userCountInMTrain->value();
	if (userCount > 0)
	{
		names n;
		n.createElements(userCount);
		n.setModal(true);
		n.show();
		takeResultAndTrain(n.exec(), userCount, n);
	}
}


void qt_gui::on_predCamStart_clicked()
{
	setState(DISABLE);
	_r.startPredictFromCam();
}


void qt_gui::on_predCamStop_clicked()
{
	_r.stopPredictFromCam();
	setState(NORMAL);
}


void qt_gui::on_multiPredCamStart_clicked()
{
	setState(DISABLE);
	_r.startMultiPredictFromCam();
}


void qt_gui::on_multiPredCamStop_clicked()
{
	_r.stopMultiPredictFromCam();
	setState(NORMAL);
}


//private


void qt_gui::takeResultAndTrain(int dlgCode, int userCount, names &n)
{
	if (dlgCode == QDialog::Accepted)
	{
		setState(DISABLE);

		std::map<int, std::string> idName;
		std::stringstream sId;
		std::stringstream sName;
		for (size_t i = 1; i <= userCount; i++)
		{
			sId.str("");
			sName.str("");
			sId << "userId" << i;
			sName << "name" << i;
			int id = n.findChild<QSpinBox*>(QString::fromStdString(sId.str()))->value();
			std::string name = n.findChild<QLineEdit*>(QString::fromStdString(sName.str()))->text().toStdString();
			idName.insert({ id, name });
		}
		_r.startMultiTrain(idName);
	}
}


void qt_gui::setState(states state)
{
	if (state == INIT) stateInit();
	if (state == NORMAL) stateNormal();
	if (state == DISABLE) stateTrain();
	if (state == TAKE_PICTURE) stateTakePicture();
	if (state == PRED) statePred();
	if (state == MULTI_PRED) stateMultiPred();
}


void qt_gui::stateInit()
{
	ui.takePictureStop->setEnabled(false);
	ui.predCamStop->setEnabled(false);
	ui.multiPredCamStop->setEnabled(false);
}


void qt_gui::stateNormal()
{
	this->setCursor(Qt::ArrowCursor);
	ui.centralWidget->setEnabled(true);
	//ui.userIdInput->setEnabled(true);
	//ui.userIdInputTrain->setEnabled(true);
	//ui.userCountInMTrain->setEnabled(true);
	setEnabledButtons(true);
	stateInit();
}


void qt_gui::stateTrain()
{
	this->setCursor(Qt::WaitCursor);
	ui.centralWidget->setEnabled(false);
}


void qt_gui::stateTakePicture()
{
	setEnabledButtons(false);
	ui.takePictureStop->setEnabled(true);
	//ui.userIdInput->setEnabled(false);
	//ui.userIdInputTrain->setEnabled(false);
	//ui.userCountInMTrain->setEnabled(false);
}


void qt_gui::statePred()
{
	setEnabledButtons(false);
	ui.predCamStop->setEnabled(true);
}


void qt_gui::stateMultiPred()
{
	setEnabledButtons(false);
	ui.multiPredCamStop->setEnabled(true);
}


void qt_gui::setEnabledButtons(bool e)
{
	ui.takePictureStart->setEnabled(e);
	ui.takePictureStop->setEnabled(e);
	ui.trainStart->setEnabled(e);
	ui.mTrainStart->setEnabled(e);
	ui.predCamStart->setEnabled(e);
	ui.predCamStop->setEnabled(e);
	ui.multiPredCamStart->setEnabled(e);
	ui.multiPredCamStop->setEnabled(e);
}
