#include "gitoid.h"

#include <gitrepository.h>

GitOid::GitOid() : QObject()
  ,m_oid({0})
  ,m_repository()
{
}

GitOid::GitOid(const git_oid *oid, GitRepository *parent) : QObject()
  ,m_oid({0})
  ,m_repository(parent)
{
    updateOid(oid);
}

GitOid::GitOid(const GitOid& other) : QObject()
{
    updateOid(&(other.m_oid));
    m_repository = other.m_repository;
}

QString GitOid::toString() const
{
    return m_string;
}

QString GitOid::toShorten() const
{
    return m_string.mid(0,9);
}

bool GitOid::operator ==(const GitOid& other) const
{
    return git_oid_equal(&m_oid, &(other.m_oid)) == true
            && m_repository == other.m_repository;
}

bool GitOid::operator <(const GitOid& other) const
{
    return git_oid_cmp(raw(), other.raw()) < 0;
}

GitOid& GitOid::operator=(const GitOid& other)
{
    updateOid(&(other.m_oid));
    m_repository = other.m_repository;
    return *this;
}

void GitOid::updateOid(const git_oid* oid)
{
    if(oid != nullptr) {
        git_oid_cpy(&m_oid, oid);
        m_string = QLatin1String(git_oid_tostr_s(&m_oid));
    } else {
        memset(&m_oid, 0, sizeof(m_oid));
        m_string = QLatin1String("");
    }
}

bool GitOid::isValid() const
{
    return !git_oid_iszero(&m_oid) && m_repository != nullptr;
}
