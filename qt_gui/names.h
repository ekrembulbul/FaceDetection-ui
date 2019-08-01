#ifndef NAMES_H
#define NAMES_H

#include "ui_names.h"
#include <map>
#include <string>

class names : public QDialog
{
    Q_OBJECT

public:
    names(QWidget *parent = Q_NULLPTR);
	void createElements(int userCount);

private:
    Ui::names ui;
};

#endif // NAMES_H
