#ifndef NOTESMODEL_H
#define NOTESMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QDir>

#include <qqmlfile.h>
#include <QtGui/QTextCharFormat>
#include <QtCore/QTextCodec>


//![0]
class Note
{
public:
    Note(const QString &path);
    //![0]

    QString title() const;
    QString category() const;
    uint datetime() const;
    QString text() const;
    QString path() const;

private:
    QString m_text;
    QString m_path;
    //![1]
};

class NotesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum NoteRoles {
        TitleRole = Qt::UserRole + 1,
        CategoryRole,
        DatetimeRole,
        PathRole
    };

    NotesModel(QObject *parent = 0);
//![1]

    void addNote(const Note &note);

    Q_INVOKABLE QString create(void) ;
    Q_INVOKABLE QString readNote(const QString &path) const;

    Q_INVOKABLE void refresh(void);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    void sort();
    void loadNotes(QDir m_notesdir);


protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QList<Note> m_notes;
//![2]
};


QDir notesFolder();
QString createNote();

#endif // NOTESMODEL_H
