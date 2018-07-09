#include "gitbranch.h"

#include <QDebug>
#include <gitrepository.h>

#include <git2.h>
namespace {
const char remoteAddtion = '/';

}

GitBranch::GitBranch() : GitReference(nullptr, nullptr)
  ,m_commit(nullptr)
  ,m_type(GitBranch::Invalid)
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
    other.m_type = GitBranch::Invalid;
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
        other.m_type = GitBranch::Invalid;
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
    if(type() == GitBranch::Remote
            || branch.type() == GitBranch::Local) {
        qWarning() << "Try to setup invalid pair of upstream/local branches";
        return;
    }
    git_branch_set_upstream(branch.raw(), branch.fullName().toUtf8().data());
}

GitBranch GitBranch::upstream() const
{
    git_reference* ref = nullptr;
    if(type() == GitBranch::Remote) {
        qDebug() << "Skipping upstream read for remote branch";
        return GitBranch();
    }
    if(git_branch_upstream(&ref, raw()) != 0) {
        qWarning() << "Invalid reference or branch type" << GitBase::lastError();
        return GitBranch();
    }
    return GitBranch(ref, GIT_BRANCH_REMOTE, repository());
}

void GitBranch::pull(PullStrategy strategy)
{
    if(type() == GitBranch::Remote) {
        qWarning() << "It's not possible to update remote branch, seems issue in branch handling logic";
        return;
    }

    GitBranch upstreamBranch = upstream();

    qDebug() << "Upstream branch for " << fullName() << " is " << upstreamBranch.fullName();

    git_annotated_commit* commit = upstreamBranch.annotatedCommit();
    git_merge_analysis_t analResult;
    git_merge_preference_t preferences;
    git_merge_analysis(&analResult, &preferences, m_repository->raw(), (const git_annotated_commit**)&commit, 1);

    if (analResult & GIT_MERGE_ANALYSIS_UP_TO_DATE) {
        qDebug() << "Up-to-date";
        return;
    }

    if (analResult & GIT_MERGE_ANALYSIS_FASTFORWARD) {
        qDebug() << "Fast-forward possible";
        fastForward();
        return;
    }

    switch (strategy) {
    case Merge:
        //TDB: Apply merge strategy
        break;
    default:
        //TDB: Apply merge strategy
        break;
    }
    qWarning() << "Other merge methods are not supported yet";
}

void GitBranch::fastForward()
{
    git_reference *newRaw = nullptr;
    if(0 == git_reference_set_target(&newRaw, m_raw, upstream().oid().raw(), "Update branch HEAD using fast-forward")) {
        m_raw = newRaw;
    } else {
        qWarning() << "Could not apply fast-forward " << lastError();
    }
}
