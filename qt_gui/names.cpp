#include "names.h"
#include <sstream>


names::names(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
}


void names::createElements(int userCount)
{
	int scrollSize = 30;
	resize(size().width(), size().height() + (userCount - 1) * scrollSize);

	std::stringstream s;
	for (size_t i = 2; i <= userCount; i++)
	{
		s.str("");
		s << "userId" << i;
		QSpinBox *tmp = new QSpinBox(this);
		tmp->setObjectName(QString::fromStdString(s.str()));
		int x = ui.userId1->x();
		int y = ui.userId1->y() + (i - 1) * scrollSize;
		int width = ui.userId1->width();
		int height = ui.userId1->height();
		tmp->setGeometry(QRect(x, y, width, height));


		// LineEdit
		s.str("");
		s << "name" << i;
		QLineEdit *tmp2 = new QLineEdit(this);
		tmp2->setObjectName(QString::fromStdString(s.str()));
		tmp2->setGeometry(QRect(ui.name1->x(), ui.name1->y() + (i - 1) * scrollSize, ui.name1->width(), ui.name1->height()));

		// Label
		s.str("");
		s << "userIdLabel" << i;
		QLabel *tmp3 = new QLabel(this);
		tmp3->setObjectName(QString::fromStdString(s.str()));
		tmp3->setGeometry(QRect(ui.userIdLabel1->x(), ui.userIdLabel1->y() + (i - 1) * scrollSize, ui.userIdLabel1->width(), ui.userIdLabel1->height()));
		tmp3->setText(QString("User ID:"));

		// Label
		s.str("");
		s << "nameLabel" << i;
		QLabel *tmp4 = new QLabel(this);
		tmp4->setObjectName(QString::fromStdString(s.str()));
		tmp4->setGeometry(QRect(ui.nameLabel1->x(), ui.nameLabel1->y() + (i - 1) * scrollSize, ui.nameLabel1->width(), ui.nameLabel1->height()));
		tmp4->setText(QString("Name:"));
	}

	ui.buttonBox->setGeometry(QRect(ui.buttonBox->x(), ui.buttonBox->y() + (userCount - 1) * scrollSize, ui.buttonBox->width(), ui.buttonBox->height()));
}