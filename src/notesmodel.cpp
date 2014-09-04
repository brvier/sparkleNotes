#include "notesmodel.h"
#include "QDir"
#include "QDateTime"
#include "QStandardPaths"
#include "QTextStream"
#include "QDebug"

#include "git2.h"

Note::Note(const QString &path)
    : m_path(path)
{
}

QString NotesModel::createNote()
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



typedef struct { /* … */ } match_data;
int match_cb(const char *path, const char *spec, void *payload)
{
    match_data *d = (match_data*)payload;
    /*
   * return 0 to add/remove this path,
   * a positive number to skip this path,
   * or a negative number to abort the operation.
   */
}

bool NotesModel::checkGitErr(int gerr) {
    if (gerr < 0) {
        const git_error *e = giterr_last();
        qDebug() << "Libgit error : %s\n" << e->message;
        emit error(QString().fromLatin1(e->message));
        return true;
    }
    return false;
}

void NotesModel::updateGitStatus() {

    git_repository *repo = NULL;
    int err = git_repository_open(&repo, notesFolder().absolutePath().toUtf8().constData());
    if (!checkGitErr(err)) {

        /* Each repository owns an index */
        git_index *idx = NULL;
        err = git_repository_index(&idx, repo);
        if (!checkGitErr(err)) {

            /* Read index from disk : FIXME Necessary ?*/
            err = git_index_read(idx, true);
            if (!checkGitErr(err)) {

                /* Set Option for git status */
                git_status_options opts = { 1, GIT_STATUS_SHOW_INDEX_AND_WORKDIR,
                                            GIT_STATUS_OPT_INCLUDE_UNTRACKED |
                                       //     GIT_STATUS_OPT_INCLUDE_UNMODIFIED |
                                            GIT_STATUS_OPT_RECURSE_UNTRACKED_DIRS };
                status_data d;
                git_status_list *statuses = NULL;
                err = git_status_list_new(&statuses, repo, &opts);
                if (!checkGitErr(err)) {

                    size_t count = git_status_list_entrycount(statuses);
                    if (count == 0) {
                        return; // No modified file, exit
                    }
                    for (size_t i=0; i<count; ++i) {
                        const git_status_entry *entry = git_status_byindex(statuses, i);

                        qDebug() << "path:" << entry->index_to_workdir->new_file.path;
                        qDebug() << "status_flags:" << entry->index_to_workdir->status;

                        /* For each new file add it to the index */
                        if ((entry->status & GIT_STATUS_WT_MODIFIED) || (entry->status & GIT_STATUS_WT_RENAMED) || (entry->status & GIT_STATUS_WT_TYPECHANGE) || entry->status == GIT_STATUS_WT_NEW) {
                            err = git_index_add_bypath(idx, entry->index_to_workdir->new_file.path);
                            checkGitErr(err);
                        }
                        else if (entry->status & GIT_STATUS_WT_DELETED) {
                            err = git_index_remove_bypath(idx, entry->index_to_workdir->old_file.path);
                            checkGitErr(err);
                        }
                    }

                    err = git_index_write(idx);
                    if (!checkGitErr(err)) {


                        /* Write the index contents to the ODB as a tree */
                        git_oid new_tree_id;
                        err = git_index_write_tree(&new_tree_id, idx);
                        if (!checkGitErr(err)) {

                            /* Create signature */
                            git_signature *me = NULL;
                            err = git_signature_now(&me, "sparkleNotes", "sparklenotes@khertan.net");
                            if (!checkGitErr(err)) {

                                /* Get tree ref */
                                git_tree *tree;
                                err = git_tree_lookup(&tree, repo, &new_tree_id);
                                if (!checkGitErr(err)) {

                                    /* Get parent commit */
                                    git_oid parentCommitId;
                                    git_commit *parent;
                                    int nparents;
                                    err = git_reference_name_to_id( &parentCommitId, repo, "HEAD" );
                                    if (err == -3) {
                                        nparents = 0;
                                    }
                                    else {
                                        err = git_commit_lookup( &parent, repo, &parentCommitId );
                                        nparents = 1;
                                    }
                                    const git_commit *parents [1] = { parent };

                                    git_oid new_commit_id;
                                    err = git_commit_create(
                                                &new_commit_id,
                                                repo,
                                                "HEAD",                      /* name of ref to update */
                                                me,                          /* author */
                                                me,                          /* committer */
                                                "UTF-8",                     /* message encoding */
                                                "Modif on notes",            /* message */
                                                tree,                        /* root tree */
                                                nparents,                    /* parent count */
                                                parents);                    /* parents */
                                    checkGitErr(err);


                                    if(nparents > 0)
                                        git_commit_free (parent);
                                    git_checkout_index (repo, idx, NULL);
                                }
                            git_signature_free(me);
                            }
                        }

                    }
                }
            }
        }
        git_index_free(idx);
    }

    git_repository_free(repo);
}


NotesModel::NotesModel(QObject *parent)
    : QAbstractListModel(parent)
{
    QDir notesdir = notesFolder();

    if (!(git_repository_open_ext(
              NULL, notesdir.absolutePath().toUtf8().constData() , GIT_REPOSITORY_OPEN_NO_SEARCH, NULL) == 0)) {
        git_repository *repo = NULL;

        // Init Git Repository if not
        int gerr = git_repository_init(&repo, notesdir.absolutePath().toUtf8().constData(), false);
        if (gerr < 0) {
            const git_error *e = giterr_last();
            qDebug() << "Libgit error : %s\n" << e->message;
            emit error(QString().fromLatin1(e->message));
        }
        git_repository_free(repo);

    }

    // Add already existing files in case created outside sparkleNotes
    updateGitStatus();


}


QString Note::title() const
{
    //qDebug() << QString(m_path);
    //qDebug() << QString(QFileInfo(m_path).baseName());
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
    //qDebug() << "path:" << m_path;
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
            }
            else {
                NotesModel::loadNotes(QDir(fileInfoList[i].absoluteFilePath()));
            }
        }
    }
    this->sort();
}

QString NotesModel::create() {
    QString path = NotesModel::createNote();
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

QHash<int, QByteArray> NotesModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[CategoryRole] = "category";
    roles[DatetimeRole] = "datetime";
    roles[PathRole] = "path";
    return roles;
}

