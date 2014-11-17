#include "qmlsettings.h"
#include "QtCore/QProcess"
#include "QtCore/QFile"
#include "QtCore/QBitArray"
#include "QtCore/QTextCodec"
#include "QtCore/QDir"
#include "QtCore/QDebug"

QMLSettings::QMLSettings(QObject *parent) :
    QSettings(parent)
{

}

void QMLSettings::set(QString key, QString value) {
    this->setValue(key, value);
}


QVariant QMLSettings::get(QString key) {
    return this->value(key);
}

QString QMLSettings::readPubKey() {
    QString pubKeyPath = this->fileName()+".key.pub";
    if (QFile::exists(pubKeyPath)) {
        QFile file(pubKeyPath);
        if (file.open(QFile::ReadOnly)) {
            QByteArray data = file.readAll();
            QTextCodec *codec = QTextCodec::codecForUtfText(data);
            return codec->toUnicode(data);
        }
    }
    return "";
}

bool QMLSettings::keygen() {

    QString pubKeyPath = this->fileName()+".key.pub";
    QString keyPath = this->fileName()+".key";

    //if not exists '~/.ssh/id_rsa.pub' then ssh-keygen -t rsa -N "" -f ~/.ssh/id_rsa
    if(QFile(pubKeyPath).exists()){
        QFile(pubKeyPath).remove();
    }
    if(QFile(keyPath).exists()){
        QFile(keyPath).remove();
    }
    QProcess process;
    process.start("/bin/uname", QProcess::ReadOnly);
    process.waitForFinished(1000);
//    qDebug() << "test:" << process.readAllStandardOutput().constData();
//    qDebug() << process.exitCode();
    QByteArray res = process.readAllStandardOutput();

    if ( res.endsWith("QNX\n")) {
        //process.start("/apps/net.khertan.sparkleNotes/native/bin/ssh-keygen", QStringList() << "-t" << "rsa" << "-N" << "" << "-f" << keyPath, QProcess::ReadOnly);
        process.start("app/native/bin/ssh-keygen", QStringList() << "-t" << "rsa" << "-N" << "" << "-f" << keyPath, QProcess::ReadOnly);
    }
    else {
        process.start("/usr/bin/ssh-keygen", QStringList() << "-t" << "rsa" << "-N" << "" << "-f" << keyPath, QProcess::ReadOnly);
    }
    process.waitForFinished(30000);
    qDebug() << process.exitCode();
    qDebug() << "ssh-keygen out:" << process.readAllStandardOutput() << process.readAllStandardError();

    if(process.error()) {
        qDebug() << "ssh-keygen out:" << process.readAllStandardOutput() << process.readAllStandardError();
        return false;
    }
    //}else{
    //    qDebug() << "Skipping keygen";
    //    return false;
    //}
    return true;
}
