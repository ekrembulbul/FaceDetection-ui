#include "qt_gui.h"
#include "names.h"


qt_gui::qt_gui(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	r.loadXml("haarcascade-frontalface-default.xml"); 
}

void qt_gui::on_takePicture_clicked()
{
	int userId = ui.userIdInput->value();
	//ui.label->setText(QString(std::to_string(userId).c_str()));
	r.takePicture(userId);
}

void qt_gui::on_train_clicked()
{
	int userId = ui.userIdInputTrain->value();
	//ui.label->setText(QString(std::to_string(userId).c_str()));
	r.train(userId);
}

void qt_gui::on_mTrain_clicked()
{
	std::map<int, std::string> idName;

	int userCount = ui.userCountInMTrain->value();
	if (userCount > 1)
	{
		int scrollSize = 30;
		names n;
		n.resize(n.size().width(), n.size().height() + (userCount - 1) * scrollSize);

		for (size_t i = 2; i <= userCount; i++)
		{
			// SpinBox
			std::stringstream s;
			s << "userId" << i;
			QSpinBox *tmp = new QSpinBox(&n);
			tmp->setObjectName(QString::fromUtf8(s.str().c_str(), s.str().size()));
			int x = n.ui.userId1->x();
			int y = n.ui.userId1->y() + (i - 1) * scrollSize;
			int width = n.ui.userId1->width();
			int height = n.ui.userId1->height();
			tmp->setGeometry(QRect(x, y, width, height));
			

			// LineEdit
			s.clear();
			s << "name" << i;
			QLineEdit *tmp2 = new QLineEdit(&n);
			tmp2->setObjectName(QString::fromUtf8(s.str().c_str(), s.str().size()));
			tmp2->setGeometry(QRect(n.ui.name1->x(), n.ui.name1->y() + (i - 1) * scrollSize, n.ui.name1->width(), n.ui.name1->height()));

			// Label
			s.clear();
			s << "userIdLabel" << i;
			QLabel *tmp3 = new QLabel(&n);
			tmp3->setObjectName(QString::fromUtf8(s.str().c_str(), s.str().size()));
			tmp3->setGeometry(QRect(n.ui.userIdLabel1->x(), n.ui.userIdLabel1->y() + (i - 1) * scrollSize, n.ui.userIdLabel1->width(), n.ui.userIdLabel1->height()));
			tmp3->setText(QString("User ID:"));

			// Label
			s.clear();
			s << "nameLabel" << i;
			QLabel *tmp4 = new QLabel(&n);
			tmp4->setObjectName(QString::fromUtf8(s.str().c_str(), s.str().size()));
			tmp4->setGeometry(QRect(n.ui.nameLabel1->x(), n.ui.nameLabel1->y() + (i - 1) * scrollSize, n.ui.nameLabel1->width(), n.ui.nameLabel1->height()));
			tmp4->setText(QString("Name:"));
		}

		n.ui.buttonBox->setGeometry(QRect(n.ui.buttonBox->x(), n.ui.buttonBox->y() + (userCount - 1) * scrollSize, n.ui.buttonBox->width(), n.ui.buttonBox->height()));

		n.setModal(true);
		n.show();

		int dlgCode = n.exec();
		//if (dlgCode == QDialog::Accepted)
		//{
		//	for (size_t i = 1; i <= userCount; i++)
		//	{
		//		idName.insert({n.ui.})
		//	}
		//}

	}

	//r.multiTrain(userCount);
}

void qt_gui::on_predCam_clicked()
{
	r.predictFromCam();
}

void qt_gui::on_multiPredCam_clicked()
{
	std::vector<std::string> names;
	r.multiPredictFromCam(names);
}

void qt_gui::on_predImage_clicked()
{
	r.predictFromImage();
}
