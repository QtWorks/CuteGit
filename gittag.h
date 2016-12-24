#ifndef GITTAG_H
#define GITTAG_H

#include <gitbaseoid.h>

class GitTag : public GitBaseOid<git_tag>
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY tagChanged)
    Q_PROPERTY(QString message READ message NOTIFY tagChanged)
    Q_PROPERTY(QString owner READ owner NOTIFY tagChanged)
    Q_PROPERTY(QString sha1 READ sha1 NOTIFY tagChanged)
    Q_PROPERTY(GitOid targetId READ targetId WRITE setTargetId NOTIFY targetIdChanged)


public:
    GitTag();
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

public slots:
    void setTargetId(GitOid targetId)
    {
        if (m_targetId == targetId)
            return;

        m_targetId = targetId;
        emit targetIdChanged(targetId);
    }

signals:
    void tagChanged();

    void targetIdChanged(GitOid targetId);

private:
    QString m_name;
    QString m_message;
    QString m_owner;
    QString m_sha1;
    GitOid m_targetId;
};

#endif // GITTAG_H
