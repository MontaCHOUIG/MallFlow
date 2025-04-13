QT += core gui sql widgets printsupport charts quick quickwidgets location positioning network

CONFIG += c++17
CONFIG += release
CONFIG -= debug_and_release

TARGET = GestionEvenements

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    evenement.cpp \
    connection.cpp

HEADERS += \
    mainwindow.h \
    evenement.h \
    connection.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    resources.qrc

# Ajouter les fichiers QML et images aux ressources
DISTFILES +=

win32 {
    LIBS += -lole32 -loleaut32 -luuid
}
unix {
    LIBS += -lpthread
}
macx {
    LIBS += -framework CoreFoundation
}

# Configuration spécifique pour QML
QML_IMPORT_PATH = qml
QML_DESIGNER_IMPORT_PATH = qml
