#include "notesmodel.h"
#include "QDir"
#include "QDateTime"
#include "QStandardPaths"
#include "QTextStream"
#include "QDebug"
#include "QSettings"
#include "git2.h"


void e(int error) {
    if (error < 0)
        throw error;
}


Note::Note(const QString &path)
    : m_path(path)
{
}

QStringList NotesModel::categoryList()
{
    return m_categories;
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
        stream << "Untitled " + QString::number(idx);
    }
    else{
        return QString();
    }
    return path + " " + QString::number(idx) + ".md";
}



/* typedef struct {  } match_data;
int match_cb(const char *path, const char *spec, void *payload)
{
    match_data *d = (match_data*)payload;

   // return 0 to add/remove this path,
   // a positive number to skip this path,
   // or a negative number to abort the operation.

}*/

bool NotesModel::checkGitErr(int gerr) {
    if (gerr < 0) {
        const git_error *e = giterr_last();
        if (e) {
            qDebug() << "Libgit error : " << e->message;
            emit error(QString().fromLatin1(e->message));
            return true;
        }
        else
        {    return false; }
    }
    return false;
}

int cred_acquire_cb(git_cred **out,
                    const char * UNUSED(url),
                    const char * UNUSED(username_from_url),
                    unsigned int UNUSED(allowed_types),
                    void * UNUSED(payload))
{

    return git_cred_ssh_key_new(out,
                                UNUSED_username_from_url,
                                QSettings().value("pubKeyPath").toString().toUtf8().constData(),
                                QSettings().value("keyPath").toString().toUtf8().constData(),
                                "");
}

static int status_cb(
        const char *ref,
        const char *msg,
        void *data)
{
    fprintf(stderr, "status ref: %s\t\tmsg: %s\n", ref, msg);
    return 0;
}

//BOUHHHHOUUUH
int NotesModel::push() {
    git_push *push;
    git_remote_callbacks callbacks = GIT_REMOTE_CALLBACKS_INIT;
    git_repository *repo = NULL;
    git_remote *remote = NULL;
    git_merge_head *merge_head = NULL;

    try {
        //Open Repo
        e(git_repository_open(&repo, notesFolder().absolutePath().toUtf8().constData()));
        e(git_remote_load(&remote, repo, "upstream"));
        callbacks.credentials = cred_acquire_cb;
        e(git_remote_set_callbacks(remote, &callbacks));
        e(git_remote_connect(remote, GIT_DIRECTION_PUSH));
        int connected = git_remote_connected(remote);
        if (connected) {
            e(git_push_new(&push, remote));

            e(git_push_add_refspec(push, "refs/heads/master:refs/heads/master"));
            e(git_push_finish(push));
            e(git_push_unpack_ok(push));
            e(git_push_status_foreach(push, &status_cb, NULL) < 0);

            git_push_free(push);
        }
    }
    catch (int error) {
        const git_error *err = giterr_last();
        if (err != NULL)
            qDebug() << QString::number(err->klass) + "\t" + QString(err->message);
        giterr_clear();
        //git_push_free(push);
        git_remote_free(remote);
        git_repository_free(repo);
    }
    return true;

}

int NotesModel::pull() {

    git_repository *repo = NULL;
    git_remote *remote = NULL;
    git_merge_head *merge_head = NULL;

    try {
        //Open Repo
        e(git_repository_open(&repo, notesFolder().absolutePath().toUtf8().constData()));

        //List repository and add one from Settings if none exists or doesn t have same URI than settings
        git_strarray   remotes = {0};
        git_remote_list(&remotes, repo);
        if (remotes.count >= 1) {
            for (size_t i = 0; i < remotes.count; i++) {
                e(git_remote_load(&remote, repo, remotes.strings[i]));
                qDebug() << git_remote_url(remote);
                if (QSettings().value("gitRemoteUrl").isValid() && git_remote_url(remote)) {
                    if (strcmp(git_remote_url(remote),QSettings().value("gitRemoteUrl").toString().toUtf8().constData()) != 0) {
                        e(git_remote_delete(remote));
                        e(git_remote_create(&remote, repo, "upstream",
                                            QSettings().value("gitRemoteUrl").toString().toUtf8().constData()));
                        e(git_remote_save(remote));
                    }

                }
            }
        } else if (remotes.count == 0) {
            e(git_remote_create(&remote, repo, "upstream",
                                QSettings().value("gitRemoteUrl").toString().toUtf8().constData()));
            e(git_remote_save(remote));
        }

        e(git_remote_load(&remote, repo, "upstream"));

        git_remote_callbacks callbacks = GIT_REMOTE_CALLBACKS_INIT;
        callbacks.credentials = cred_acquire_cb;
        e(git_remote_set_callbacks(remote, &callbacks));

        e(git_remote_connect(remote, GIT_DIRECTION_FETCH));
        int connected = git_remote_connected(remote);
        if (connected)
            e(git_remote_fetch(remote, NULL, NULL));


        git_checkout_options checkout_opt = GIT_CHECKOUT_OPTIONS_INIT;
        checkout_opt.checkout_strategy = GIT_CHECKOUT_FORCE;
        checkout_opt.file_mode = 0644;
        git_merge_options merge_opt = GIT_MERGE_OPTIONS_INIT;
        merge_opt.file_favor = GIT_MERGE_FILE_FAVOR_UNION;

        const git_remote_head **head = NULL;
        size_t size = 0;
        e(git_remote_ls(&head, &size, remote));
        if (size <= 0)
            e(-1);

        git_oid oid = head[0]->oid;

        e(git_merge_head_from_fetchhead(&merge_head, repo, "master", git_remote_url(remote), &oid));
        e(git_merge(repo, (const git_merge_head **)(&merge_head), 1, &merge_opt, &checkout_opt));

        //TRY TO COMMIT
        /* Create signature */
        git_signature *me = NULL;
        e(git_signature_now(&me, "sparkleNotes", "sparklenotes@khertan.net"));

        //Tree Lookup
        git_tree *tree;
        git_object *tree_obj;
        e(git_revparse_single(&tree_obj, repo, "HEAD^{tree}"));

        // Get parent commit
        git_oid parentCommitId;
        git_commit *parent;
        git_oid remoteParentCommitId;
        git_commit *remoteParent;
        int nparents;
        int err;
        e(git_reference_name_to_id( &parentCommitId, repo, "ORIG_HEAD" ));
        e(git_commit_lookup( &parent, repo, &parentCommitId ));
        e(git_reference_name_to_id( &remoteParentCommitId, repo, "MERGE_HEAD" ));
        e(git_commit_lookup( &remoteParent, repo, &remoteParentCommitId ));

        if (remoteParent == parent) {
            //Same commit ... nothing to merge
            e(git_repository_state_cleanup(repo));
            git_merge_head_free(merge_head);
            git_remote_free(remote);
            git_repository_free(repo);
            return false;
        }
        const git_commit *parents [2] = { parent, remoteParent };

        git_oid new_commit_id;
        e(git_commit_create(
              &new_commit_id,
              repo,
              "HEAD",                      /* name of ref to update */
              me,                          /* author */
              me,                          /* committer */
              "UTF-8",                     /* message encoding */
              "Merge remote upstream/master",            /* message */
              (git_tree *) tree_obj,                        /* root tree */
              2,                    /* parent count */
              parents));                    /* parents */


        git_merge_head_free(merge_head);
        git_remote_free(remote);

        e(git_repository_state_cleanup(repo));

        git_repository_free(repo);

    }

    catch (int error) {
        const git_error *err = giterr_last();
        if (err != NULL)
            qDebug() << QString::number(err->klass) + "\t" + QString(err->message);
        giterr_clear();
        git_merge_head_free(merge_head);
        git_remote_free(remote);
        git_repository_free(repo);
    }
    return true;
}


void NotesModel::updateGitStatus() {
    git_repository *repo = NULL;
    git_index *idx = NULL;
    git_signature *me = NULL;

    try {
        e(git_repository_open(&repo, notesFolder().absolutePath().toUtf8().constData()));

        /* Each repository owns an index */
        e(git_repository_index(&idx, repo));

        /* Read index from disk : FIXME Necessary ?*/
        e(git_index_read(idx, true));


        /* Set Option for git status */
        git_status_options opts = { 1, GIT_STATUS_SHOW_INDEX_AND_WORKDIR,
                                    GIT_STATUS_OPT_INCLUDE_UNTRACKED |
                                    //     GIT_STATUS_OPT_INCLUDE_UNMODIFIED |
                                    GIT_STATUS_OPT_RECURSE_UNTRACKED_DIRS };
        //status_data d;
        int state = git_repository_state(repo);

        git_status_list *statuses = NULL;
        e(git_status_list_new(&statuses, repo, &opts));


        size_t count = git_status_list_entrycount(statuses);
        if ((count == 0) && (GIT_REPOSITORY_STATE_MERGE != state)) {
            return; // No modified file, exit
        }
        for (size_t i=0; i<count; ++i) {
            const git_status_entry *entry = git_status_byindex(statuses, i);

            /* For each new file add it to the index */
            if ((entry->status & GIT_STATUS_WT_MODIFIED) || (entry->status & GIT_STATUS_WT_RENAMED) || (entry->status & GIT_STATUS_WT_TYPECHANGE) || entry->status == GIT_STATUS_WT_NEW) {
                e(git_index_add_bypath(idx, entry->index_to_workdir->new_file.path));
            }
            else if (entry->status & GIT_STATUS_WT_DELETED) {
                e(git_index_remove_bypath(idx, entry->index_to_workdir->old_file.path));
            }
        }

        e(git_index_write(idx));


        /* Write the index contents to the ODB as a tree */
        git_oid new_tree_id;
        e(git_index_write_tree(&new_tree_id, idx));

        /* Create signature */
        e(git_signature_now(&me, "sparkleNotes", "sparklenotes@khertan.net"));


        /* Get tree ref */
        git_tree *tree;
        e(git_tree_lookup(&tree, repo, &new_tree_id));

        /* Get parent commit */
        git_oid parentCommitId;
        git_commit *parent;
        int nparents;
        int err;
        err = git_reference_name_to_id( &parentCommitId, repo, "HEAD" );
        if (err == -3) {
            nparents = 0;
        }
        else {
            e(git_commit_lookup( &parent, repo, &parentCommitId ));
            nparents = 1;
        }
        const git_commit *parents [1] = { parent };

        git_oid new_commit_id;
        e(git_commit_create(
              &new_commit_id,
              repo,
              "HEAD",                      /* name of ref to update */
              me,                          /* author */
              me,                          /* committer */
              "UTF-8",                     /* message encoding */
              "Modif on notes",            /* message */
              tree,                        /* root tree */
              nparents,                    /* parent count */
              parents));                    /* parents */


        if(nparents > 0)
            git_commit_free (parent);
        git_checkout_index (repo, idx, NULL);
        git_signature_free(me);
        git_index_free(idx);
        git_repository_free(repo);
    }
        catch (int error) {
            const git_error *err = giterr_last();
            if (err != NULL)
                qDebug() << QString::number(err->klass) + "\t" + QString(err->message);
            giterr_clear();
            git_signature_free(me);
            git_index_free(idx);
            git_repository_free(repo);
        }


}


NotesModel::NotesModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_categories=QStringList(QString("All"));
    m_filter=QString("All");

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

        //Create Initial Commit
        git_signature *sig;
        git_index *index;
        git_oid tree_id, commit_id;
        git_tree *tree;

        gerr = git_signature_now(&sig, "sparkleNotes", "sparklenotes@khertan.net");

        gerr = git_repository_index(&index, repo);
        if (!checkGitErr(gerr)) {
            gerr = git_index_write_tree(&tree_id, index);
            if (!checkGitErr(gerr)) {

                git_index_free(index);

                gerr = git_tree_lookup(&tree, repo, &tree_id);
                if (!checkGitErr(gerr)) {

                    gerr = git_commit_create_v(
                                &commit_id, repo, "HEAD", sig, sig,
                                NULL, "Initial commit", tree, 0);
                }
                git_tree_free(tree);
            }
        }
        git_signature_free(sig);
        git_repository_free(repo);

    }

    // Add already existing files in case created outside sparkleNotes
    updateGitStatus();
    QtConcurrent::run(this, &NotesModel::pullMergePush);

}

QString appendPath(const QString& path1, const QString& path2)
{
    return QDir::cleanPath(path1 + QDir::separator() + path2);
}

void NotesModel::pullMergePush() {
    // Pull Merge
    pull();

    // Push
    push();

}

QString Note::title() const
{
    return QFileInfo(m_path).baseName();
}

QString Note::category() const
{
    QString cat = QFileInfo(m_path).dir().dirName();
    if (cat == notesFolder().dirName())
        return "";
    return cat;
}

QString Note::setCategory(QString cat) const
{
    QString new_cat_path = appendPath(notesFolder().absolutePath(), cat);
    QString new_file_path = appendPath(new_cat_path,
                                       QFileInfo(m_path).fileName());

    QDir new_cat_dir(new_cat_path);
    if (!new_cat_dir.exists())
        new_cat_dir.mkpath(new_cat_path);

    QFile tomove(m_path);
    tomove.rename(new_file_path);

    return new_file_path;
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

QString NotesModel::setCategory(const QString &path, const QString &category)  {
    Note note(path);
    qDebug() << "NotesModel2::setCategory";
    qDebug() << path;
    QString newPath(note.setCategory(category));
    this->refresh(true);
    return newPath;
}

bool NotesModel::deleteNote(const QString &path)  {
    qDebug() << "NotesModel::deleteNote";
    bool ret = QFile(path).remove();
    this->refresh(true);
    return ret;
}

void NotesModel::addNote(const Note &note)
{   
    if (!m_categories.contains(note.category()) && (!note.category().isEmpty())) {
        m_categories.append(note.category());
    }

    if ((m_filter == "All") || (m_filter == note.category())) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        m_notes << note;
        endInsertRows();
    }
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

void NotesModel::refresh(const bool full) {
    if (full) {
        updateGitStatus();
        QtConcurrent::run(this, &NotesModel::pullMergePush);
    }
    beginResetModel();
    m_notes.clear();
    loadNotes(notesFolder());
    endResetModel();
}

void NotesModel::setFilter(const QString &filter) {
    m_filter = filter;
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

