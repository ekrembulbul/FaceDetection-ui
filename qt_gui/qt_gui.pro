# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

TEMPLATE = app
TARGET = qt_gui
DESTDIR = ../x64/Debug
QT += core gui widgets
CONFIG += debug
DEFINES += _UNICODE _ENABLE_EXTENDED_ALIGNED_STORAGE WIN64 QT_DLL QT_WIDGETS_LIB
INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/$(ConfigurationName)
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/$(ConfigurationName)
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(qt_gui.pri)

FORMS += \
    names.ui

HEADERS += \
    names.h

SOURCES += \
    names.cpp
