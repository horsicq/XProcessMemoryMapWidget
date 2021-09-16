INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/xprocessmemorymapwidget.ui

HEADERS += \
    $$PWD/xprocessmemorymapwidget.h

SOURCES += \
    $$PWD/xprocessmemorymapwidget.cpp

!contains(XCONFIG, xprocess) {
    XCONFIG += xprocess
    include($$PWD/../XProcess/xprocess.pri)
}

!contains(XCONFIG, xbinary) {
    XCONFIG += xbinary
    include($$PWD/../Formats/xbinary.pri)
}
