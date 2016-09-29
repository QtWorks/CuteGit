#ifndef GITHANDLER_H
#define GITHANDLER_H

#include <QObject>

#include <gitrepository.h>

class GitHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(GitRepository* repo READ repo WRITE setRepo NOTIFY repoChanged)

public:
    GitHandler();
    virtual ~GitHandler();
    Q_INVOKABLE void open(const QString &path);
    Q_INVOKABLE void open(const QUrl &url);
    Q_INVOKABLE void close();

    GitRepository* repo() const
    {
        return m_repo;
    }

public slots:
    void setRepo(GitRepository* repo)
    {
        if (m_repo == repo)
            return;

        m_repo = repo;
        emit repoChanged(repo);
    }

signals:
    void repoChanged(GitRepository* repo);

protected:
    QString lastError() const;
private:
    GitRepository* m_repo;
};

#endif // GITHANDLER_H
