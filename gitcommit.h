#ifndef GITCOMMIT_H
#define GITCOMMIT_H

#include <gitbase.h>

#include <QDateTime>
#include <QString>

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
    Q_PROPERTY(int x READ x NOTIFY commitChanged)
    Q_PROPERTY(int y READ y NOTIFY commitChanged)
    Q_PROPERTY(QString color READ color)

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
    int x() const { return m_x; }
    int y() const { return m_y; }
    QString color() const { return m_color; }

public slots:
    void setAuthor(QString author);
    void setTime(QDateTime time);
    void setMessage(QString message);
    void setEmail(QString email);

signals:
    void commitChanged();

private:
    GitCommit();

    QString m_author;
    QDateTime m_time;
    QString m_message;
    QString m_email;

public:
    int m_x;
    int m_y;
    int m_childrenCounter;
    QString m_color;
};

#endif // GITCOMMIT_H
