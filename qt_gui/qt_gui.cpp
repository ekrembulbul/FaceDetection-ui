#include "qt_gui.h"
#include "names.h"
#include <map>
#include <string>


qt_gui::qt_gui(QWidget *parent)
	: QMainWindow(parent), _isFirstInNameDialog(true)
{
	ui.setupUi(this);
}


void qt_gui::on_takePictureStart_clicked()
{
	ui.takePictureStart->setEnabled(false);
	ui.trainStart->setEnabled(false);
	ui.mTrainStart->setEnabled(false);
	ui.predCamStart->setEnabled(false);
	ui.predCamStop->setEnabled(false);
	ui.multiPredCamStart->setEnabled(false);
	ui.multiPredCamStop->setEnabled(false);

	int userId = ui.userIdInput->value();
	_r.startTakePicture(userId);
}


void qt_gui::on_takePictureStop_clicked()
{
	_r.stopTakePicture();

	ui.takePictureStart->setEnabled(true);
	ui.trainStart->setEnabled(true);
	ui.mTrainStart->setEnabled(true);
	ui.predCamStart->setEnabled(true);
	ui.predCamStop->setEnabled(true);
	ui.multiPredCamStart->setEnabled(true);
	ui.multiPredCamStop->setEnabled(true);
}


void qt_gui::on_trainStart_clicked()
{
	int userId = ui.userIdInputTrain->value();
	_r.startTrain(userId);
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
