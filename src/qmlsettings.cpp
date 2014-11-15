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
    process.start("/usr/bin/ssh-keygen", QStringList() << "-t" << "rsa" << "-N" << "" << "-f" << keyPath);
    process.waitForFinished(30000);
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
