#-------------------------------------------------
#
# Project created by QtCreator 2014-01-02T09:56:07
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ServerPlate
TEMPLATE = app


INCLUDEPATH += /usr/local/include/opencv /usr/local/include .

LIBS += -L/usr/local/lib                                \
        -L/usr/local/lib -lrt -lpthread -lm -ldl        \
        /usr/local/lib/libcvblob.so                     \
        -Wl,--rpath=.



SOURCES += main.cpp\
        mainwindow.cpp \
    ORM/DbManager.cpp \
    ORM/Image.cpp \
    ORM/Log.cpp \
    ORM/Plate.cpp \
    ORM/Test.cpp \
    Utils/TraceLogger.cpp \
    Server.cpp


SOURCES += process_image/DetectRegions_cmyk.cpp
SOURCES += process_image/DetectRegions.cpp
SOURCES += process_image/OCR.cpp
SOURCES += process_image/img_Plate.cpp
SOURCES += process_image/Segmentation.cpp
SOURCES += process_image/Utils.cpp

SOURCES += process_image/QtImageWindow.cpp


# SOURCES += process_image/main.cpp


HEADERS  += mainwindow.h \
    ORM/DbManager.hpp \
    ORM/Image.hpp \
    ORM/Log.hpp \
    ORM/ORM_Log.hpp \
    ORM/ORM.hpp \
    ORM/Plate.hpp \
    ORM/Test.hpp \
    Utils/TraceLogger.hpp \
    Protocole.hpp \
    Server.hpp


HEADERS  += process_image/QtImageWindow.hpp

HEADERS  += process_image/DetectRegions.hpp
HEADERS  += process_image/OCR.hpp
HEADERS  += process_image/img_Plate.hpp
HEADERS  += process_image/Segmentation.hpp
HEADERS  += process_image/Utils.hpp


FORMS    += mainwindow.ui

