#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "documenthandler.h"
#include "notesmodel.h"
#include <QtCore/QSettings>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setApplicationName("sparkleNotes");
    app.setApplicationVersion("0.0.1");
    app.setOrganizationName("Khertan");

    NotesModel notesModel;
    notesModel.loadNotes(notesFolder());

    QSettings settings;

    QQmlApplicationEngine engine;
    qmlRegisterType<DocumentHandler>("net.khertan.documenthandler", 1, 0, "DocumentHandler");
    engine.rootContext()->setContextProperty("notesModel", &notesModel);
    engine.rootContext()->setContextProperty("settings", &settings);

    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));
    return app.exec();
}
