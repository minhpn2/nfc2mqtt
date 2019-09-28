QT -= gui
QT +=  network core concurrent mqtt positioning
CONFIG += c++11

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

include(hidread/hidapi/hidapi.pri)
INCLUDEPATH += $$PWD/lib
LIBS += -L/usr/lib -ludev

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += "$$PWD/../../../sysroot/" \
               "$$PWD/../../../sysroot/lib" \
               "$$PWD/../../../sysroot/opt" \
               "$$PWD/../../../sysroot/usr" \
               "$$PWD/../../../sysroot/usr/include" \
               "$$PWD/../../../sysroot/usr/include/arm-linux-gnueabihf"

LIBS +=    "-L$$PWD/../../../sysroot/" \
           "-L$$PWD/../../../sysroot/opt" \
           "-L$$PWD/../../../sysroot/lib" \
           "-L$$PWD/../../../sysroot/usr" \
           "-L$$PWD/../../../sysroot/usr/include" \
           "-L$$PWD/../../../sysroot/usr/include/arm-linux-gnueabihf"


SOURCES += \
        main.cpp \
    maincontroller.cpp \
    hidread/devicemanager.cpp \
    hidread/hidrawdevice.cpp \
    configsetting.cpp \
    mqttconfig.cpp \
    mqttsender.cpp \
    nfcsender.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /usr/local/qt5pi/vpos
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    maincontroller.h \
    hidread/devicemanager.h \
    hidread/hidrawdevice.h \
    configsetting.h \
    mqttconfig.h \
    mqttsender.h \
    nfcsender.h
