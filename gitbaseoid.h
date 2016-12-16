#ifndef GITBASEOID_H
#define GITBASEOID_H

#include <gitbase.h>
#include <gitoid.h>

#include <git2/oid.h>

template<typename T>
class GitBaseOid : public GitBase<T>
{
public:
    GitBaseOid(T* raw, GitRepository* parent) : GitBase<T>(raw, parent)
      ,m_oid(nullptr, parent)
    {}

    const GitOid& oid() const {
        return m_oid;
    }
protected:
    GitOid m_oid;
};

#endif // GITBASEOID_H
