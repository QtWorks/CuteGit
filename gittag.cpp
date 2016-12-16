#include "gittag.h"
#include <git2/tag.h>

GitTag::GitTag(git_tag *raw, GitRepository *parent) : GitBaseOid(raw, parent)
{
    if(raw == nullptr) {
        return;
    }

    const git_oid* oid = git_tag_id(raw);
    m_oid = GitOid(oid, parent);
    m_name = QString::fromUtf8(git_tag_name(raw));
}

GitOid GitTag::targetId() const
{
    return GitOid(git_tag_target_id(m_raw), repository());
}
