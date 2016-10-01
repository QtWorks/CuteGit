#include "gitbranch.h"

#include <QDebug>
#include <gitrepository.h>

#include <git2.h>

GitBranch::GitBranch(git_reference *ref, GitRepository *parent) : GitReference(ref, parent)
  ,m_commit(nullptr)
{
    char oid_buf[GIT_OID_HEXSZ+1];
    const char* tmpName;
    git_branch_name(&tmpName, m_raw);
    m_name = tmpName;
    git_annotated_commit_from_ref(&m_commit, repository()->raw(), m_raw);
    const git_oid* tmpOid = git_annotated_commit_id(m_commit);
    memcpy(m_oid, tmpOid, sizeof(m_oid));
    git_oid_tostr(oid_buf, GIT_OID_HEXSZ+1,branch_oid);
    qDebug() << oid_buf;
    //        m_branches.insert(out)

}

GitBranch::~GitBranch()
{
    if(m_commit != nullptr) {
        git_annotated_commit_free(m_commit);
    }
}
