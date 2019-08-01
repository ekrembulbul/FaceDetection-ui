#include "qt_gui.h"
#include "names.h"
#include <map>
#include <string>
#include <qdebug.h>
#include <Windows.h>


qt_gui::qt_gui(QWidget *parent)
	: QMainWindow(parent), _isFirstInNameDialog(true)
{
	ui.setupUi(this);
	ui.videoLabel->setPixmap(QPixmap::fromImage(QImage("icon/camera.png")));
	connect(&_r, &recognizer::readyImage, this, &qt_gui::drawImage);
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
	int userId = ui.userIdInput->value();
	_r.startTakePicture(userId);
}


void qt_gui::on_takePictureStop_clicked()
{
	_r.stopTakePicture();
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
	int userId = ui.userIdInputTrain->value();
	_r.startTrain(userId);
}


void qt_gui::on_trainStop()
{

}


void qt_gui::on_mTrainStart_clicked()
{
	int userCount = ui.userCountInMTrain->value();
	if (userCount > 1)
	{
		_n.createElements(userCount, _isFirstInNameDialog);
		_isFirstInNameDialog = false;
		_n.setModal(true);
		_n.show();
		takeResultAndTrain(_n.exec(), userCount);
	}
}


void qt_gui::on_predCamStart_clicked()
{
	_r.startPredictFromCam();
}


void qt_gui::on_predCamStop_clicked()
{
	_r.stopPredictFromCam();
}


void qt_gui::on_multiPredCamStart_clicked()
{
	_r.startMultiPredictFromCam();
}


void qt_gui::on_multiPredCamStop_clicked()
{
	_r.stopMultiPredictFromCam();
}


//private


void qt_gui::takeResultAndTrain(int dlgCode, int userCount)
{
	if (dlgCode == QDialog::Accepted)
	{
		std::map<int, std::string> idName;
		std::stringstream sId;
		std::stringstream sName;
		for (size_t i = 1; i <= userCount; i++)
		{
			sId.str("");
			sName.str("");
			sId << "userId" << i;
			sName << "name" << i;
			int id = _n.findChild<QSpinBox*>(QString::fromStdString(sId.str()))->value();
			std::string name = _n.findChild<QLineEdit*>(QString::fromStdString(sName.str()))->text().toStdString();
			idName.insert({ id, name });
		}
		_r.startMultiTrain(idName);
	}
}
