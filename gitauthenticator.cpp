#include "gitauthenticator.h"
#include <QDir>
#include <QDebug>

#include <git2/transport.h>

namespace {
QString DefaultPublicKey = QDir::homePath() + QString("/.ssh/id_rsa.pub");
QString DefaultPrivateKey = QDir::homePath() + QString("/.ssh/id_rsa");
}

GitAuthenticator::GitAuthenticator()
{

}

int GitAuthenticator::authenticate(GitRemote *remote, git_cred **credentials, const char *usernameFromUrl, unsigned int allowedTypes)
{
    if(allowedTypes & GIT_CREDTYPE_SSH_KEY) {
        return authenticateSsh(remote, credentials, usernameFromUrl);
    } else {
        qWarning() << "Only ssh key-based authentication possible";
    }
    return -1;
}

int GitAuthenticator::authenticateSsh(GitRemote* remote, git_cred **credentials, const char* usernameFromUrl)
{
    //TODO: here might be advanced logic for key authentication
    return git_cred_ssh_key_new(credentials, usernameFromUrl, DefaultPublicKey.toUtf8().data(), DefaultPrivateKey.toUtf8().data(), "");
}
