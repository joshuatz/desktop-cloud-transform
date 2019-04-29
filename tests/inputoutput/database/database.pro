# Include GoogleTest
include(../../gtest_dependency.pri)
#LIBS += -lgtest
# Include app src
include(../../../qmake-vars.pri)
INCLUDEPATH += $$PROJECTAPPDIR

# Special QT includes
QT += sql
QT += network

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
#CONFIG -= qt
CONFIG += qt

HEADERS += \
        tst_database.h \
        $$PROJECTAPPDIR/appinfo.h \
        $$PROJECTAPPDIR/helpers.h \
        $$PROJECTAPPDIR/database.h \
        $$PROJECTAPPDIR/globalsettings.h \
        $$PROJECTAPPDIR/../lib/mousaviexecutesqlfile.h

SOURCES += \
        main.cpp \
        $$PROJECTAPPDIR/helpers.cpp \
        $$PROJECTAPPDIR/database.cpp \
        $$PROJECTAPPDIR/globalsettings.cpp

include($$PROJECTAPPDIR/models/models.pri)
include($$PROJECTAPPDIR/apis/apis.pri)
include($$PROJECTAPPDIR/../lib/qt-google-analytics/qt-google-analytics.pri)
