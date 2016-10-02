#include "gitcommit.h"

#include <gitoid.h>

#include <QDebug>

#include <git2/commit.h>


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
