#-------------------------------------------------
#
# Project created by QtCreator 2014-04-30T10:42:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RTSPTool
TEMPLATE = app


SOURCES += main.cpp\
        frmmain.cpp \
    qffmpeg.cpp \
    rtspthread.cpp

HEADERS  += frmmain.h \
    ffmpeg/include/libavcodec/avcodec.h \
    ffmpeg/include/libavfilter/avfilter.h \
    ffmpeg/include/libavformat/avformat.h \
    ffmpeg/include/libavutil/avutil.h \
    ffmpeg/include/libswscale/swscale.h \
    qffmpeg.h \
    rtspthread.h

FORMS    += frmmain.ui
####LIBS += -L[path to lib] -l[name of lib]
###LIBS += -L$$PWD/lib/ -ltest
INCLUDEPATH +=  ffmpeg/include
LIBS += -L$$PWD/ffmpeg/lib/ -llibavcodec\
        -L$$PWD/ffmpeg/lib/ -llibavfilter\
        -L$$PWD/ffmpeg/lib/ -llibavformat\
        -L$$PWD/ffmpeg/lib/ -llibswscale\
        -L$$PWD/ffmpeg/lib/ -llibavutil

MOC_DIR=temp/moc
RCC_DIR=temp/rcc
UI_DIR=temp/ui
OBJECTS_DIR=temp/obj
DESTDIR=bin

win32:RC_FILE=main.rc

RESOURCES += \
    res.qrc
