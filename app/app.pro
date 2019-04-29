QT += core gui
QT += qml
QT += quick
QT += quickcontrols2
QT += sql
QT += network
CONFIG += c++11
CONFIG += qtquickcompiler

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

include("../config/extradefines.pro")
include(../qmake-vars.pri)


# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    $$PROJECTAPPDIR/main.cpp \
    $$PROJECTAPPDIR/helpers.cpp \
    $$PROJECTAPPDIR/database.cpp \
    $$PROJECTAPPDIR/globalsettings.cpp

HEADERS += \
    $$PROJECTAPPDIR/appinfo.h \
    $$PROJECTAPPDIR/helpers.h \
    $$PROJECTAPPDIR/database.h \
    $$PROJECTAPPDIR/globalsettings.h \
    $$PROJECTAPPDIR/../lib/mousaviexecutesqlfile.h

RESOURCES += qml.qrc

include("models/models.pri")
include("apis/apis.pri")
include("../lib/qt-google-analytics/qt-google-analytics.pri")

# Additional import path used to resolve QML modules in Qt Creator's code model
# I shouldn't have to do this...
#QML_IMPORT_PATH = C:\Qt\5.12.1\mingw73_64\qml

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Icon
RC_ICONS = ../transform_352180.ico

# Debug vs release stuff
CONFIG (debug, debug|release) {
    CONFIG += qml_debug
    CONFIG += console
    message("Debug Mode On!")
}
else {
    DEFINES += QT_NO_DEBUG_OUTPUT
    message("Release mode.")
}
