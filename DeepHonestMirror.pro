#-------------------------------------------------
#
# Project created by QtCreator 2017-05-23T14:55:37
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DeepHonestMirror
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    cvcamera.cpp \
    roi_analyzer.cpp \
    classifier.cpp \
    facedetector.cpp

HEADERS  += mainwindow.h \
    cvcamera.h \
    roi_analyzer.h \
    classifier.h \
    labels.h \
    facedetector.h

FORMS    += mainwindow.ui

# add open CV
unix {
    CONFIG += link_pkgconfig c++11
    PKGCONFIG += opencv
}

# cuda
INCLUDEPATH += /usr/local/cuda/include
LIBS += -L/usr/local/cuda/lib64
LIBS += -lcudart -lcublas -lcurand

INCLUDEPATH += /home/simonli/age_gender/caffe/include \
               /home/simonli/age_gender/caffe/src \
               /home/simonli/age_gender/caffe/build/src \
               /usr/lib/x86_64-linux-gnu \
               /usr/local/include

LIBS += -L/home/simonli/age_gender/caffe/build/lib  \
        -lglog          \
        -lboost_system  \
        -lcaffe
