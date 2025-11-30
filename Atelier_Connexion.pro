QT += core gui sql printsupport widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = Atelier_Connexion
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    connection.cpp \
    parking.cpp

HEADERS += \
    mainwindow.h \
    connection.h \
    parking.h

FORMS += \
    mainwindow.ui
```
RESOURCES += resources.qrc

DISTFILES +=




