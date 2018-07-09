#ifndef GITBASE_H
#define GITBASE_H

#include <gitrepository.h>

#include <QObject>
#include <git2/types.h>
#include <git2/errors.h>

class GitRepository;

template <typename T>
class GitBase : public QObject
{
public:
    T *raw() const {
        return m_raw;
    }

    bool isValid() const {
        return m_raw != nullptr;
    }

    GitRepository *repository() const {
        return m_repository;
    }

    static QString lastError() {
        const git_error *e = giterr_last();
        if(e) {
            return QString("(%1): %2").arg(e->klass).arg(e->message);
            giterr_clear();
        }
        giterr_clear();
        return QString();
    }

protected:
    GitBase(T *raw, GitRepository *parent) : QObject()
      ,m_raw(raw)
      ,m_repository(parent)
    {}

    GitBase(GitBase &&other) {
        //WARNING: m_raw should be moved in inherited class
        m_repository = other.m_repository;
        other.m_repository = nullptr;
    }

    GitBase &operator=(GitBase &&other) {
        if (&other != this) {
            //WARNING: m_raw should be moved in inherited class
            m_repository = other.m_repository;
            other.m_repository = nullptr;
        }
        return *this;
    }

    T *m_raw;
    GitRepository *m_repository;
};

#endif // GITBASE_H
