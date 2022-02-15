QT       += concurrent

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

FORMS += \
    $$PWD/dialogxprocessmemorymap.ui \
    $$PWD/xprocessmemorymapwidget.ui

HEADERS += \
    $$PWD/dialogxprocessmemorymap.h \
    $$PWD/xprocessmemorymapwidget.h

SOURCES += \
    $$PWD/dialogxprocessmemorymap.cpp \
    $$PWD/xprocessmemorymapwidget.cpp

!contains(XCONFIG, xprocess) {
    XCONFIG += xprocess
    include($$PWD/../XProcess/xprocess.pri)
}

!contains(XCONFIG, xbinary) {
    XCONFIG += xbinary
    include($$PWD/../Formats/xbinary.pri)
}
