#-------------------------------------------------
#
# Project created by QtCreator 2020-08-03T18:14:55
#
#-------------------------------------------------

QT       += core gui serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProtocolsTestTool
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    SerialTestWidget.cpp \
    ProtocolDefine.cpp \
    TcpTestWidget.cpp \
    UdpTestWidget.cpp

HEADERS  += MainWindow.h \
    SerialTestWidget.h \
    ProtocolDefine.h \
    TcpTestWidget.h \
    UdpTestWidget.h

FORMS    += MainWindow.ui \
    SerialTestWidget.ui \
    TcpTestWidget.ui \
    UdpTestWidget.ui
