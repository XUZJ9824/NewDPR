QT += quick

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    src/dx_iface.cpp \
    src/maploader.cpp

RESOURCES += qml.qrc

INCLUDEPATH += libiconv/include
INCLUDEPATH += libdxfrw/src

TRANSLATIONS += \
    NewDPR_en_US.ts

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    libiconv/include/encodings.def \
    libiconv/include/encodings_aix.def \
    libiconv/include/encodings_dos.def \
    libiconv/include/encodings_extra.def \
    libiconv/include/encodings_local.def \
    libiconv/include/encodings_osf1.def \
    libiconv/include/translit.def

HEADERS += \
    src/dx_data.h \
    src/dx_iface.h \
    src/maploader.h

win32: LIBS += -L$$PWD/libdxfrw/bin/ -llibdxfrw
win32: LIBS += -L$$PWD/libdxfrw/vs/packages/libiconv.1.14.0.11/build/native/lib/v110/x64/Release/dynamic/cdecl/ -llibiconv_dynamic
win32: LIBS += -L$$PWD/libdxfrw/vs/packages/libiconv.1.14.0.11/build/native/lib/v110/x64/Release/static/cdecl/ -llibiconv_static

INCLUDEPATH += $$PWD/''
DEPENDPATH += $$PWD/''
