#ifndef GITBASE_H
#define GITBASE_H

#include <gitrepository.h>
#include <gitoid.h>

#include <QObject>
#include <git2/types.h>
#include <git2/oid.h>

class GitRepository;

template <typename T>
class GitBase : public QObject
{
public:
    GitBase(T* raw, GitRepository* parent) : QObject(parent)
      ,m_raw(raw)
      ,m_repository(parent)
      ,m_oid(nullptr, parent)
    {}

    T* raw() const {
        return m_raw;
    }

    bool isValid() const {
        return m_raw != nullptr;
    }

    GitRepository* repository() const {
        return m_repository;
    }

    const GitOid& oid() const {
        return m_oid;
    }

protected:
    T* m_raw;
    GitRepository* m_repository;
    GitOid m_oid;
};

#endif // GITBASE_H