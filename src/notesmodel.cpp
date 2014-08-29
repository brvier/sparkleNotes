#include "notesmodel.h"
#include "QDir"
#include "QDateTime"
#include "QStandardPaths"
#include "QTextStream"
#include "QDebug"

Note::Note(const QString &path)
    : m_path(path)
{
}

QString createNote()
{
    QDir notefolder = notesFolder();
    QString path = notefolder.absoluteFilePath("Untitled");
    int idx = 1;
    while (notefolder.exists(path + " " + QString::number(idx) + ".md")) {
        idx++;
    }
    QFile fnote(path + " " + QString::number(idx) + ".md");
    if (fnote.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&fnote);
        stream << "Untitled";
    }
    else{
        return QString();
    }
    return path + " " + QString::number(idx) + ".md";
}

NotesModel::NotesModel(QObject *parent)
    : QAbstractListModel(parent)
{
}


QString Note::title() const
{
    qDebug() << QString(m_path);
    qDebug() << QString(QFileInfo(m_path).baseName());
    return QFileInfo(m_path).baseName();
}

QString Note::category() const
{
    QString cat = QFileInfo(m_path).dir().dirName();
    if (cat == notesFolder().dirName())
        return "";
    return cat;
}

uint Note::datetime() const
{
    return QFileInfo(m_path).lastModified().toTime_t();
}

QString Note::path() const
{
    qDebug() << "path:" << m_path;
    return m_path;
}


void NotesModel::addNote(const Note &note)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_notes << note;
    endInsertRows();
}

QDir notesFolder() {
    QDir notesdir = QDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    if (!notesdir.exists())
        notesdir.mkpath(".");
    return notesdir;
}

bool dtcomp(Note left, Note right)
{
    if (left.category() == right.category()) {
        return left.datetime() > right.datetime(); }
    else {
        return left.category() < right.category();
    }
}


void NotesModel::sort() {
    beginResetModel();
    qSort(m_notes.begin(), m_notes.end(), dtcomp);
    endResetModel();
}

void NotesModel::refresh() {
    beginResetModel();
    m_notes.clear();
    loadNotes(notesFolder());
    endResetModel();
}

void NotesModel::loadNotes(QDir notesdir) {
    QFileInfoList fileInfoList = notesdir.entryInfoList();
    for (int i=0; i<fileInfoList.count(); i++) {
        if ((fileInfoList[i].fileName()!=".") && (fileInfoList[i].fileName()!="..")) {
            if (fileInfoList[i].isFile()) {
                NotesModel::addNote(Note(fileInfoList[i].absoluteFilePath()));
                qDebug() << "FP:" << fileInfoList[i].absoluteFilePath();
            }
            else {
                NotesModel::loadNotes(QDir(fileInfoList[i].absoluteFilePath()));
            }
        }
    }
    this->sort();
}

QString NotesModel::create() {
    QString path = createNote();
    NotesModel::addNote(Note(path));
    NotesModel::sort();
    return path;
}

int NotesModel::rowCount(const QModelIndex & parent) const {
    return m_notes.count();
}

QString NotesModel::readNote(const QString &path) const {
    qDebug() << "readNote:" << path;
    QString fileName = QQmlFile::urlToLocalFileOrQrc(path);
    if (QFile::exists(fileName)) {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly)) {
            QByteArray data = file.readAll();
            QTextCodec *codec = QTextCodec::codecForHtml(data);
            return codec->toUnicode(data);
        }
    }
    return QString();
}

QVariant NotesModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_notes.count())
        return QVariant();

    const Note &note = m_notes[index.row()];
    if (role == TitleRole)
        return note.title();
    else if (role == CategoryRole)
        return note.category();
    else if (role == DatetimeRole)
        return note.datetime();
    else if (role == PathRole)
        return note.path();
    return QVariant();
}

//![0]
QHash<int, QByteArray> NotesModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[CategoryRole] = "category";
    roles[DatetimeRole] = "datetime";
    roles[PathRole] = "path";
    return roles;
}
//![0]

