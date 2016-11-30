#include "gitcommit.h"

#include <gitoid.h>

#include <QDebug>

#include <git2/commit.h>
#include <git2/tag.h>
#include <git2/diff.h>
#include <git2/patch.h>
#include <git2/buffer.h>

GitCommit::GitCommit(git_commit* raw, GitRepository* parent) : GitBase(raw, parent)
{
    m_oid = GitOid(git_commit_id(m_raw), m_repository);
}

GitCommit::GitCommit() : GitBase(nullptr, nullptr)
{
}

GitCommit* GitCommit::fromOid(const GitOid& oid)
{
    if(!oid.isValid()) {
        return nullptr;
    }

    git_commit *commit;
    if(git_commit_lookup(&commit, oid.repository()->raw(), oid.raw()) != 0) {
        return nullptr;
    }

    return new GitCommit(commit, oid.repository());
}

GitCommit::~GitCommit()
{
    git_commit_free(m_raw);
}

QString GitCommit::author() const
{
    return QString(git_commit_author(m_raw)->name);
}

QDateTime GitCommit::time() const
{
    return QDateTime::fromMSecsSinceEpoch(git_commit_time(m_raw));
}

QString GitCommit::message() const
{
    return QString(git_commit_message(m_raw));
}

QString GitCommit::email() const
{
    return QString(git_commit_author(m_raw)->email);
}

QString GitCommit::sha1() const
{
    return oid().toString();
}

QString GitCommit::shortSha1() const
{
    return oid().toShorten();
}

bool GitCommit::isMerge() const
{
    return git_commit_parentcount(m_raw) > 1;
}

QString GitCommit::body()
{
    if(isMerge()) {
        return QString("Commit - merge");
    }


    if(m_body.isEmpty() || m_body.isNull()) {
        git_commit *parent = nullptr;
        git_commit_parent(&parent, raw(), 0);

        git_tree *commit_tree = nullptr, *parent_tree = nullptr;
        git_commit_tree(&commit_tree, raw());
        git_commit_tree(&parent_tree, parent);

        git_diff *diff = nullptr;
        git_diff_tree_to_tree(
                    &diff, repository()->raw()  , parent_tree, commit_tree, nullptr);


       git_diff_print(diff,
                      GIT_DIFF_FORMAT_PATCH,
                        [](const git_diff_delta *delta, /**< delta that contains this data */
                      const git_diff_hunk *hunk,   /**< hunk containing this data */
                      const git_diff_line *line,   /**< line data */
                      void *payload)->int
        {
           qDebug() << "GIT_DELTA_ADDED" << delta->status;
//           qDebug() << hunk->new_lines;
//           qDebug() << hunk->old_lines;
//            qDebug() << line->new_lineno;
            return 0;
        }, this);
//        m_body = QString::fromUtf8(git_diff_(raw()));
    }
    return m_body;
}

void GitCommit::setAuthor(QString author)
{
    Q_UNUSED(author)
    //TODO
}

void GitCommit::setTime(QDateTime time)
{
    Q_UNUSED(time)
    //TODO
}

void GitCommit::setMessage(QString message)
{
    Q_UNUSED(message)
    //TODO
}

void GitCommit::setEmail(QString email)
{
    Q_UNUSED(email)
    //TODO
}
