FORMS += qonvertwidget.ui progresswidget.ui
HEADERS += qonvert.h progress.h
SOURCES += qonvert.cpp progress.cpp main.cpp
RESOURCES += qonvert.qrc
TEMPLATE = app
CONFIG += release warn_off thread qt
TARGET = qonvert
RC_FILE = qonvert.rc
