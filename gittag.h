#ifndef GITTAG_H
#define GITTAG_H

#include <gitbase.h>

class GitTag : public GitBase<git_tag>
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY tagChanged)
    Q_PROPERTY(QString message READ message NOTIFY tagChanged)
    Q_PROPERTY(QString owner READ owner NOTIFY tagChanged)
    Q_PROPERTY(QString sha1 READ sha1 NOTIFY tagChanged)

public:
    GitTag(git_tag* raw, GitRepository* parent);

    QString name() const
    {
        return m_name;
    }

    QString message() const
    {
        return m_message;
    }

    QString owner() const
    {
        return m_owner;
    }

    QString sha1() const
    {
        return m_oid.toString();
    }

    GitOid targetId() const;

signals:
    void tagChanged();

private:
    QString m_name;
    QString m_message;
    QString m_owner;
    QString m_sha1;
};

#endif // GITTAG_H
