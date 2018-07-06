#ifndef GITREMOTE_H
#define GITREMOTE_H

#include <QObject>
#include <gitbase.h>

struct git_remote;

class GitRemote : public GitBase<git_remote>
{
    Q_OBJECT
public:
    GitRemote(git_remote* raw, GitRepository* parent);
    static GitRemote* fromName(const QString& remoteName, GitRepository* parent);

    void fetch();

    QString name() const;
signals:

private:
};

#endif // GITREMOTE_H
