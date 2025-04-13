QT       += core gui sql  # Ajout du module SQL
QT += sql printsupport  # Ajoutez printsupport
QT += multimedia widgets
QT += core gui widgets multimedia
QT += charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# Émettre des avertissements pour les fonctionnalités obsolètes
DEFINES += QT_DEPRECATED_WARNINGS

# SOURCES - fichiers source
SOURCES += \
    connection.cpp \
    main.cpp \
    mainwindow.cpp \
    services.cpp \
    urgence.cpp

# HEADERS - fichiers d'en-tête
HEADERS += \
    connection.h \
    mainwindow.h \
    services.h \
    urgence.h

# FORMS - fichiers UI
FORMS += \
    mainwindow.ui

# Règles de déploiement par défaut
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Fichiers additionnels à inclure dans le projet
DISTFILES += \
    imgs/output-onlinepngtools (2).png \
    resources/background.jpeg \
    resources/background.png

# Inclusion des ressources
RESOURCES += \
    resources.qrc
