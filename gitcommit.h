#ifndef GITCOMMIT_H
#define GITCOMMIT_H

#include <gitbase.h>
#include <gittag.h>

#include <QDateTime>
#include <QString>
#include <QList>

#include <git2/types.h>

class GitCommit : public GitBase<git_commit>
{
    Q_OBJECT
    Q_PROPERTY(QString author READ author WRITE setAuthor NOTIFY commitChanged)
    Q_PROPERTY(QString email READ email WRITE setEmail NOTIFY commitChanged)
    Q_PROPERTY(QDateTime time READ time WRITE setTime NOTIFY commitChanged)
    Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY commitChanged)
    Q_PROPERTY(QString sha1 READ sha1 NOTIFY commitChanged)
    Q_PROPERTY(QString shortSha1 READ shortSha1 NOTIFY commitChanged)
    Q_PROPERTY(bool isMerge READ isMerge NOTIFY commitChanged)
    Q_PROPERTY(QString body READ body NOTIFY bodyChanged)

public:
    GitCommit(git_commit* raw, GitRepository* parent);
    ~GitCommit();

    static GitCommit* fromOid(const GitOid& oid);

    QString author() const;
    QDateTime time() const;
    QString message() const;
    QString email() const;
    QString sha1() const;
    QString shortSha1() const;
    bool isMerge() const;
    QString body();

public slots:
    void setAuthor(QString author);
    void setTime(QDateTime time);
    void setMessage(QString message);
    void setEmail(QString email);

signals:
    void commitChanged();

    void bodyChanged(QString body);

private:
    GitCommit();

    QString m_author;
    QDateTime m_time;
    QString m_message;
    QString m_email;
    QString m_body;
};

#endif // GITCOMMIT_H
