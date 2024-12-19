QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += /common

SOURCES += \
    client\abonentManager.cpp \
    client\chatWindow.cpp \
    client\interface.cpp \
    main.cpp \
    common\communicator.cpp \
    common\common.cpp \
    server\abonent.cpp \
    server\ats.cpp \
    server\person.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    client\abonentManager.h \
    client\chatWindow.h \
    client\interface.h \
    common\communicator.h \
    common\common.h \
    server\abonent.h \
    server\ats.h \
    server\person.h
