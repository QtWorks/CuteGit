#include "gitbranch.h"

#include <QDebug>
#include <gitrepository.h>

#include <git2.h>

GitBranch::GitBranch(git_reference *ref, git_branch_t type, GitRepository *parent) : GitReference(ref, parent)
  ,m_commit(nullptr)
  ,m_type(static_cast<BranchType>(type))
{
    const char* tmpName;
    git_branch_name(&tmpName, m_raw);
    m_name = tmpName;
    git_annotated_commit_from_ref(&m_commit, repository()->raw(), m_raw);
    m_oid = GitOid(git_annotated_commit_id(m_commit), m_repository);
    qDebug() << m_oid.toString();
}

GitBranch::~GitBranch()
{
    if(m_commit != nullptr) {
        git_annotated_commit_free(m_commit);
    }
}

GitBranch::BranchType GitBranch::type() const
{
    return m_type;
}
