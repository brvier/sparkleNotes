#ifndef NOTESMODEL_H
#define NOTESMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QDir>

#include <qqmlfile.h>
#include <QtGui/QTextCharFormat>
#include <QtCore/QTextCodec>


class Note
{
public:
    Note(const QString &path);

    QString title() const;
    QString category() const;
    uint datetime() const;
    QString text() const;
    QString path() const;

private:
    QString m_text;
    QString m_path;
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

    void addNote(const Note &note);

    Q_INVOKABLE QString create(void) ;
    Q_INVOKABLE QString readNote(const QString &path) const;

    Q_INVOKABLE void refresh(void);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    void sort();
    void loadNotes(QDir m_notesdir);
    void updateGitStatus();

Q_SIGNALS:
    void error(QString);

protected:
    QHash<int, QByteArray> roleNames() const;
    bool checkGitErr(int gerr);

private:
    QList<Note> m_notes;
    QString createNote();
};

QDir notesFolder();


typedef struct { } status_data;

#endif
