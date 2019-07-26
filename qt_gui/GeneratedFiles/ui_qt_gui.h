/********************************************************************************
** Form generated from reading UI file 'qt_gui.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QT_GUI_H
#define UI_QT_GUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_qt_guiClass
{
public:
    QAction *actionExit;
    QAction *actionAbout;
    QWidget *centralWidget;
    QPushButton *pushButton;
    QLabel *label;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *qt_guiClass)
    {
        if (qt_guiClass->objectName().isEmpty())
            qt_guiClass->setObjectName(QString::fromUtf8("qt_guiClass"));
        qt_guiClass->resize(513, 407);
        actionExit = new QAction(qt_guiClass);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionAbout = new QAction(qt_guiClass);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        centralWidget = new QWidget(qt_guiClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(210, 50, 75, 23));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(220, 170, 47, 13));
        qt_guiClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(qt_guiClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        qt_guiClass->setStatusBar(statusBar);
        menuBar = new QMenuBar(qt_guiClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 513, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        qt_guiClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(qt_guiClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        qt_guiClass->addToolBar(Qt::TopToolBarArea, mainToolBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionExit);
        menuHelp->addAction(actionAbout);

        retranslateUi(qt_guiClass);
        QObject::connect(actionExit, SIGNAL(triggered()), qt_guiClass, SLOT(close()));
        QObject::connect(pushButton, SIGNAL(clicked()), label, SLOT(update()));

        QMetaObject::connectSlotsByName(qt_guiClass);
    } // setupUi

    void retranslateUi(QMainWindow *qt_guiClass)
    {
        qt_guiClass->setWindowTitle(QCoreApplication::translate("qt_guiClass", "Face Recognizer", nullptr));
        actionExit->setText(QCoreApplication::translate("qt_guiClass", "Exit", nullptr));
        actionAbout->setText(QCoreApplication::translate("qt_guiClass", "About", nullptr));
        pushButton->setText(QCoreApplication::translate("qt_guiClass", "PushButton", nullptr));
        label->setText(QString());
        menuFile->setTitle(QCoreApplication::translate("qt_guiClass", "File", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("qt_guiClass", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class qt_guiClass: public Ui_qt_guiClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QT_GUI_H
