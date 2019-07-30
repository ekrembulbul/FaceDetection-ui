#include "qt_gui.h"
#include "names.h"
#include <map>
#include <string>


qt_gui::qt_gui(QWidget *parent)
	: QMainWindow(parent), _isFirstIn(true)
{
	ui.setupUi(this);
	_r.loadXml(std::string("haarcascade-frontalface-default.xml"));
}


void qt_gui::on_takePicture_clicked()
{
	int userId = ui.userIdInput->value();
	_r.takePicture(userId);
}


void qt_gui::on_train_clicked()
{
	int userId = ui.userIdInputTrain->value();
	_r.train(userId);
}


void qt_gui::on_mTrain_clicked()
{
	int userCount = ui.userCountInMTrain->value();
	if (userCount > 1)
	{
		_n.createElements(userCount, _isFirstIn);
		_isFirstIn = false;
		_n.setModal(true);
		_n.show();
		takeResultAndTrain(_n.exec(), userCount);
	}
}


void qt_gui::on_predCam_clicked()
{
	_r.predictFromCam();
}


void qt_gui::on_multiPredCam_clicked()
{
	_r.multiPredictFromCam();
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
		_r.multiTrain(idName);
	}
}
