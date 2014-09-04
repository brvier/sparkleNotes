TEMPLATE = app

QT += qml quick

HEADERS += src/documenthandler.h \
    src/highlighter.h \
    src/notesmodel.h

SOURCES += src/main.cpp \
    src/documenthandler.cpp \
    src/highlighter.cpp \
    src/notesmodel.cpp

RESOURCES += qml/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

OTHER_FILES += \
    bar-descriptor.xml

LIBS += -L$$PWD/libgit2-build/ -lgit2

INCLUDEPATH += $$PWD/libgit2/include
DEPENDPATH += $$PWD/libgit2/include
