

TEMPLATE = app
QT += network widgets
TARGET = client
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += Client.hpp \
    Protocole.hpp
FORMS += FenClient.ui
SOURCES += Client.cpp main_c.cpp


