QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH +=  $$PWD/ffmpeg/include
LIBS +=         $$PWD/ffmpeg/lib/libavformat.dll.a \
                $$PWD/ffmpeg/lib/libavcodec.dll.a \
                $$PWD/ffmpeg/lib/libavdevice.dll.a \
                $$PWD/ffmpeg/lib/libavfilter.dll.a \
                $$PWD/ffmpeg/lib/libavutil.dll.a \
                $$PWD/ffmpeg/lib/libpostproc.dll.a \
                $$PWD/ffmpeg/lib/libswresample.dll.a \
                $$PWD/ffmpeg/lib/libswscale.dll.a
SOURCES += \
    ffmpegwidget.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ffmpegwidget.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
