#-------------------------------------------------
#
# Project created by QtCreator 2015-04-10T14:24:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BigFileWordCounterQt
TEMPLATE = app


SOURCES += \
    Ui/WordCounterWindow.cpp \
    Ui/WordSortingProgressDialog.cpp \
    Ui/WordTableModel.cpp \
    WordCounting/ExternalWordSorter.cpp \
    WordCounting/FilePartCreator.cpp \
    WordCounting/IndexingStreamWriter.cpp \
    WordCounting/NumericKWayMerger.cpp \
    WordCounting/NumericWordSorter.cpp \
    WordCounting/WordCountingKWayMerger.cpp \
    WordCounting/WordCountingSorter.cpp \
    Main.cpp

HEADERS  += \
    test.h \
    Ui/WordCounterWindow.h \
    Ui/WordSortingProgressDialog.h \
    Ui/WordTableModel.h \
    WordCounting/ExternalWordSorter.h \
    WordCounting/FilePartCreator.h \
    WordCounting/IndexingStreamWriter.h \
    WordCounting/NumericKWayMerger.h \
    WordCounting/NumericWordSorter.h \
    WordCounting/WordCountingKWayMerger.h \
    WordCounting/WordCountingSorter.h \
    StdAfx.h

DISTFILES += \
    Readme.txt

FORMS += \
    Ui/WordCounter.ui \
    Ui/WordCounterProgressDialog.ui
