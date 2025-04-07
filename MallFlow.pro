# Définir la version de Qt et les modules requis
QT += core gui sql widgets printsupport


# Configuration du projet
CONFIG += c++17
CONFIG += release
CONFIG -= debug_and_release

# Fichiers sources
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    evenement.cpp \
    connection.cpp

# Fichiers d'en-tête
HEADERS += \
    mainwindow.h \
    evenement.h \
    connection.h

# Fichiers d'interface utilisateur (UI)
FORMS += \
    mainwindow.ui

# Fichiers de ressources
RESOURCES += \
    resources.qrc

# Options de compilation
QMAKE_CXXFLAGS += -Wall -Wextra -O2

# Nom de l'exécutable généré
TARGET = GestionEvenements

# Configuration spécifique aux plateformes
win32 {
    LIBS += -lole32 -loleaut32 -luuid
}

unix {
    LIBS += -lpthread
}

macx {
    LIBS += -framework CoreFoundation
}

# Dossiers supplémentaires pour les includes et dépendances
INCLUDEPATH += include
DEPENDPATH += include
