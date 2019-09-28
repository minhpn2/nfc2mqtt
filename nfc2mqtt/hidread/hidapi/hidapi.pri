# -------------------------------------------------
# Project created by QtCreator 2010-10-29T15:54:07
# -------------------------------------------------

# -------------------------------------------------
# Add appropriate source file depending on OS
# -------------------------------------------------
#macx:  SOURCES += $$PWD/mac/hid.c
unix: !macx:   {
#SOURCES += $$PWD/libusb/hid.c
SOURCES += $$PWD/linux/hid.c
}
win32: SOURCES += $$PWD/windows/hid.cpp

HEADERS += \
    $$PWD/hidapi/hidapi.h

INCLUDEPATH += $$PWD/hidapi
