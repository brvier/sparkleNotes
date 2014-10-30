#include "qmlsettings.h"

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
