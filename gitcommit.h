#ifndef GITCOMMIT_H
#define GITCOMMIT_H

#include <gitbaseoid.h>
#include <gittag.h>

#include <QDateTime>
#include <QString>
#include <QList>

#include <git2/types.h>

class GitDiff;

class GitCommit : public GitBaseOid<git_commit>
{
    Q_OBJECT
    Q_PROPERTY(QString author READ author WRITE setAuthor NOTIFY commitChanged)
    Q_PROPERTY(QString email READ email WRITE setEmail NOTIFY commitChanged)
    Q_PROPERTY(QDateTime time READ time WRITE setTime NOTIFY commitChanged)
    Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY commitChanged)
    Q_PROPERTY(QString summary READ summary WRITE setSummary NOTIFY commitChanged)
    Q_PROPERTY(QString sha1 READ sha1 NOTIFY commitChanged)
    Q_PROPERTY(QString shortSha1 READ shortSha1 NOTIFY commitChanged)
    Q_PROPERTY(bool isMerge READ isMerge NOTIFY commitChanged)
    Q_PROPERTY(GitDiff* diff READ diff NOTIFY commitChanged)

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
    QString summary() const;
    GitDiff* diff();

public slots:
    void setAuthor(QString author);
    void setTime(QDateTime time);
    void setMessage(QString message);
    void setEmail(QString email);
    void setSummary(QString summary);

signals:
    void commitChanged();

    void bodyChanged(QString body);

private:
    GitCommit();

/*
 * TODO: Seems cached values are not used. Don't remember why it's done this way.
 * But it's better to remove them.
 */
//    QString m_author;
//    QDateTime m_time;
//    QString m_message;
//    QString m_email;
//    QString m_summary;

    QPointer<GitDiff> m_diff;
};

#endif // GITCOMMIT_H
