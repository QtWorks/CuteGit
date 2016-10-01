#ifndef GITOID_H
#define GITOID_H

#include <QObject>
#include <git2/oid.h>

class GitOid : public QObject
{
    Q_OBJECT
public:
    explicit GitOid(const git_oid* oid = 0, QObject *parent = 0);
    QString toString() const;
private:
    git_oid m_oid;
    bool isValid;
};

#endif // GITOID_H
