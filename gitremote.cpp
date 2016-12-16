#include "gitremote.h"
#include <git2/remote.h>

GitRemote::GitRemote(git_remote* raw, GitRepository* parent) : GitBase(raw, parent)
{

}

GitRemote* GitRemote::fromName(const QString& remoteName, GitRepository* parent)
{
    qDebug() << "new remote: " << remoteName;
    git_remote* remote = nullptr;
    git_remote_lookup(&remote, parent->raw(), remoteName.toUtf8().data());

    return new GitRemote(remote, parent);
}
