#include "gitbranch.h"

#include <QDebug>
#include <gitrepository.h>

#include <git2.h>
namespace {
const char remoteAddtion = '/';

}

GitBranch::GitBranch() : GitReference(nullptr, nullptr)
  ,m_commit(nullptr)
  ,m_type(Invalid)
{
}

GitBranch::GitBranch(git_reference *ref, git_branch_t type, GitRepository *parent) : GitReference(ref, parent)
  ,m_commit(nullptr)
  ,m_type(static_cast<BranchType>(type))
{
    const char* tmpName;
    git_branch_name(&tmpName, m_raw);
    m_fullName = tmpName;
    m_name = m_fullName;

    git_annotated_commit_from_ref(&m_commit, repository()->raw(), m_raw);
    m_oid = GitOid(git_annotated_commit_id(m_commit), m_repository);

    git_buf buf = GIT_BUF_INIT_CONST("",0);
    if(git_branch_remote_name(&buf, repository()->raw(), refName().toUtf8().data()) == 0) {
        m_remote = QString::fromUtf8(buf.ptr);
        m_name.remove(m_remote + remoteAddtion);
    }
}

GitBranch::GitBranch(GitBranch&& other) : GitReference(std::move(other))
{
    //TODO: looks like free() functionality might be more common for GitBase childred.
    //Need to think about it
    if(m_commit != nullptr) {
        git_annotated_commit_free(m_commit);
        m_commit = nullptr;
    }
    m_commit = other.m_commit;
    other.m_commit = nullptr;
}

GitBranch &GitBranch::operator=(GitBranch&& other)
{
    if(&other != this) {
        if(m_commit != nullptr) {
            git_annotated_commit_free(m_commit);
            m_commit = nullptr;
        }
        m_commit = other.m_commit;
        other.m_commit = nullptr;
    }
    return static_cast<GitBranch&>(GitReference::operator=(std::move(other)));
}

GitBranch::~GitBranch()
{
    if(m_commit != nullptr) {
        git_annotated_commit_free(m_commit);
        m_commit = nullptr;
    }
}

GitBranch::BranchType GitBranch::type() const
{
    return m_type;
}

void GitBranch::setUpstream(const GitBranch& branch)
{
    if(type() == GIT_BRANCH_REMOTE
            || branch.type() == GIT_BRANCH_LOCAL) {
        qWarning() << "Try to setup invalid pair of upstream/local branches";
        return;
    }
    git_branch_set_upstream(branch.raw(), branch.fullName().toUtf8().data());
}

GitBranch GitBranch::upstream() const
{
    git_reference* ref = nullptr;
    if(type() == GIT_BRANCH_REMOTE) {
        qDebug() << "Skipping upstream read for remote branch";
        return GitBranch();
    }
    if(git_branch_upstream(&ref, raw()) != 0) {
        qWarning() << "Invalid reference or branch type" << GitBase::lastError();
        return GitBranch();
    }
    return GitBranch(ref, GIT_BRANCH_REMOTE, repository());
}
