#include "gitrepository.h"

#include <QDebug>
#include <QFileInfo>
#include <QDir>

#include <gitbranch.h>

#include <git2.h>

char oid_buf[GIT_OID_HEXSZ+1];

GitRepository::GitRepository(const QString& root) : QObject(nullptr)
{
    if(git_repository_open(&m_raw, root.toUtf8().data()) != 0) {
        qDebug() << "Cannot open repository";
        close();
        return;
    }

    m_root = root;
    m_path = git_repository_workdir(m_raw);
    m_name = m_path;//TODO: replace with Human readable name
    qDebug() << "New repo:" << m_name << m_root << m_path;

    //    git_reflog* reflog;

    //    if(git_reflog_read(&reflog, m_raw, "HEAD") != 0) {
    //        qDebug() << "reflogs could not be read";
    //        return;
    //    }

    //    quint64 count = git_reflog_entrycount(reflog);
    //    qDebug() << count;

    //    for(quint64 i = 0; i < count; i++)
    //    {
    //        git_reflog_entry* entry = git_reflog_entry_byindex(reflog, i);
    //        git_oid* oid = git_reflog_entry_id_new(entry);
    //    }
    //    git_reflog_free(reflog);

    git_reference *out;
    git_branch_t branch;
    git_branch_iterator* iter;
    git_branch_iterator_new(&iter, m_raw, GIT_BRANCH_ALL);

    git_revwalk* walk;
    git_revwalk_new(&walk, m_raw);
    qDebug() << "Branches found:";
    while(git_branch_next(&out, &branch, iter) == 0)
    {
        GitBranch testBranch(out, this);
        qDebug() << testBranch.name();
    }

    return;

    git_revwalk_push_glob(walk, "refs/heads/*");
    git_revwalk_sorting(walk, GIT_SORT_TIME);

    git_oid newoid;
    while(git_revwalk_next(&newoid, walk) == 0)
    {
        git_commit *wcommit;
        if(git_commit_lookup(&wcommit, m_raw, &newoid) != 0 )
        {
            qDebug() << "git_commit_lookup error";
            continue;
        }

        const git_oid* commit_oid = git_commit_id(wcommit);
        git_oid_tostr(oid_buf,GIT_OID_HEXSZ+1,commit_oid);
        qDebug() << oid_buf;
        qDebug() << git_commit_time( wcommit );
        qDebug() << git_commit_message( wcommit );
        qDebug() << git_commit_author( wcommit );

        qDebug() << "=================================================";
        git_commit_free( wcommit );
    }

    git_revwalk_free( walk );

}

GitRepository::~GitRepository()
{
    close();
}

void GitRepository::close()
{
    if(m_raw) {
        git_repository_free(m_raw);
    }
    m_raw = nullptr;
}
