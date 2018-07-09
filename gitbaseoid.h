#ifndef GITBASEOID_H
#define GITBASEOID_H

#include <gitbase.h>
#include <gitoid.h>

#include <git2/oid.h>

template<typename T>
class GitBaseOid : public GitBase<T>
{
public:
    const GitOid &oid() const {
        return m_oid;
    }

protected:
    GitBaseOid(T *raw, GitRepository *parent) : GitBase<T>(raw, parent)
      ,m_oid(nullptr, parent)
    {}

    GitBaseOid(GitBaseOid &&other) : GitBase<T>(std::move(other))
    {
        m_oid = other.m_oid;
        other.m_oid = GitOid();
    }

    GitBaseOid &operator=(GitBaseOid &&other) {
        if (&other != this) {
            m_oid = other.m_oid;
            other.m_oid = GitOid();
        }
        return static_cast<GitBaseOid&>(GitBase<T>::operator=(std::move(other)));
    }

    GitOid m_oid;
};

#endif // GITBASEOID_H
