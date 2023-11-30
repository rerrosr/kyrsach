

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QT_Project_On_Movie_Ticket_Booking
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    mydb.cpp \
    book.cpp \
    cancel.cpp

HEADERS += \
        mainwindow.h \
    mydb.h \
    book.h \
    cancel.h

FORMS += \
        mainwindow.ui \
    book.ui \
    cancel.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
