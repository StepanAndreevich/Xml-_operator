QT       += core gui sql xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    database.cpp \
    fileimporter.cpp \
    logmodel.cpp \
    main.cpp \
    mainwindow.cpp \
    parser.cpp \
    progressbarwidget.cpp \
    tablemodel.cpp

HEADERS += \
    database.h \
    fileimporter.h \
    logmodel.h \
    mainwindow.h \
    parser.h \
    progressbarwidget.h \
    tablemodel.h

FORMS += \
    mainwindow.ui \
    progressbarwidget.ui

TRANSLATIONS += \
    OperatorXml_ru_RU.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
