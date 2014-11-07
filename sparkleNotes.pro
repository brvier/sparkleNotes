TEMPLATE = app

QT += qml quick
QT += network

HEADERS += src/documenthandler.h \
    src/highlighter.h \
    src/notesmodel.h \
    src/qmlsettings.h

SOURCES += src/main.cpp \
    src/documenthandler.cpp \
    src/highlighter.cpp \
    src/notesmodel.cpp \
    src/qmlsettings.cpp

RESOURCES += qml/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

OTHER_FILES += \
    bar-descriptor.xml

!qnx:LIBS += -L$$PWD/../libgit2-desktop/ -lgit2
qnx:LIBS += -L$$PWD/../libgit2/ -lgit2
qnx:LIBS += -L$$PWD/../libssh2/ -lssh2

INCLUDEPATH += $$PWD/../libgit2/include
DEPENDPATH += $$PWD/../libgit2/include

INCLUDEPATH += $$PWD/../libssh2/include
DEPENDPATH += $$PWD/../libssh2/include

