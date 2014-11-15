#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "documenthandler.h"
#include "notesmodel.h"
#include "qmlsettings.h"
#include <QtCore/QSettings>
#include <git2.h>
#include <QStringList>

#define DEBUGLOG_PORT 10467

#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) /*@unused@*/ x
#else
# define UNUSED(x) x
#endif

void send_message(QtMsgType type,
                  const QMessageLogContext& ctxt,
                  const QString& msg) {


    QUdpSocket *socket = new QUdpSocket(new QObject());

    QString mesg = "" +  msg;
    fprintf(stderr, mesg.toStdString().c_str());

    int r = socket->writeDatagram(mesg.toStdString().c_str(),
                          mesg.toStdString().size(),
                          QHostAddress::Broadcast,
                          DEBUGLOG_PORT);
    socket->waitForBytesWritten(5000);
    socket->close();
}



int main(int argc, char *argv[])
{
    QString __version__ = "0.0.1";

    QGuiApplication app(argc, argv);

    app.setApplicationName("sparkleNotes");
    app.setApplicationVersion(__version__);
    app.setOrganizationName("Khertan");

    //qInstallMessageHandler(send_message);

    qDebug() << "Starting SparkleNotes";

    NotesModel notesModel;
    notesModel.loadNotes(notesFolder());

    QMLSettings settings;

    QQmlApplicationEngine engine;
    qmlRegisterType<DocumentHandler>("net.khertan.documenthandler", 1, 0, "DocumentHandler");
    engine.rootContext()->setContextProperty("notesModel", &notesModel);
    engine.rootContext()->setContextProperty("settings", &settings);
    engine.rootContext()->setContextProperty("__version__", __version__);

    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));
    return app.exec();
}
