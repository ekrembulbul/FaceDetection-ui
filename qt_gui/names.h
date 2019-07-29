#ifndef NAMES_H
#define NAMES_H

#include "ui_names.h"
#include <QDialog>

class names : public QDialog
{
    Q_OBJECT

public:
    names(QWidget *parent = Q_NULLPTR);
    Ui::names ui;

private:
};

#endif // NAMES_H
