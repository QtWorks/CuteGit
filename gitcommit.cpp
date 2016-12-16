#include "gitcommit.h"

#include <gitoid.h>
#include <gitdiff.h>

#include <QDebug>

#include <git2/commit.h>
#include <git2/tag.h>
#include <git2/diff.h>
#include <git2/patch.h>
#include <git2/buffer.h>

GitCommit::GitCommit(git_commit* raw, GitRepository* parent) : GitBaseOid(raw, parent)
  ,m_diff(nullptr)
{
    m_oid = GitOid(git_commit_id(m_raw), m_repository);
}

GitCommit::GitCommit() : GitBaseOid(nullptr, nullptr)
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
    return QDateTime::fromTime_t(git_commit_time(m_raw), Qt::OffsetFromUTC, git_commit_time_offset(m_raw));
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

QString GitCommit::summary() const
{
    return QString(git_commit_summary(m_raw));
}

GitDiff* GitCommit::diff()
{
    if(isMerge()) {
        return nullptr;//QString("Commit - merge");
    }

    if(m_diff.isNull()) {
        git_commit *parent = nullptr;
        git_commit_parent(&parent, raw(), 0);
        m_diff = new GitDiff(parent, raw(), repository());
        git_commit_free(parent);
    }
    return m_diff.data();
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

void GitCommit::setSummary(QString summary)
{
    Q_UNUSED(summary)
    //TODO
}
