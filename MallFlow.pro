QT       += core gui sql widgets charts printsupport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = EmployeeSponsorApp
TEMPLATE = app

# Emit warnings for deprecated Qt features
DEFINES += QT_DEPRECATED_WARNINGS

QT += sql
QT += charts
QT += network
QT += printsupport

SOURCES += \
    connection.cpp \
    main_application.cpp \
    src/employees/employees_mainwindow.cpp \
    src/employees/employe.cpp \
    src/employees/modifieremploye.cpp \
    src/employees/securityquestion.cpp \
    src/employees/emailsender.cpp \
    src/employees/login.cpp \
    src/sponsors/sponsors_mainwindow.cpp \
    src/sponsors/sponsor.cpp \
    main.cpp

HEADERS += \
    connection.h \
    main_application.h \
    src/employees/employees_mainwindow.h \
    src/employees/employe.h \
    src/employees/modifieremploye.h \
    src/employees/securityquestion.h \
    src/employees/emailsender.h \
    src/employees/login.h \
    src/sponsors/sponsors_mainwindow.h \
    src/sponsors/sponsor.h

FORMS += \
    src/employees/employees.ui \
    src/sponsors/sponsors.ui \
    main_application.ui

RESOURCES += \
    resources/resources.qrc

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    resources/imgs/output_onlinepngtools_2.png \
    resources/imgs/output_onlinepngtools.png \
    resources/imgs/background.png \
    resources/imgs/logo.png \
    resources/styles/style.qss
