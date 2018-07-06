#ifndef GITAUTHENTICATOR_H
#define GITAUTHENTICATOR_H

#include <QObject>

class GitRemote;
struct git_cred;

class GitAuthenticator : public QObject
{
    Q_OBJECT

public:
    int authenticate(GitRemote *remote, git_cred **credentials, const char *usernameFromUrl, unsigned int allowedTypes);
    static GitAuthenticator* instance() {
        static GitAuthenticator s_instance;
        return &s_instance;
    }
private:
    GitAuthenticator();
    int authenticateSsh(GitRemote* remote, git_cred **credentials, const char* usernameFromUrl);
};

#endif // GITAUTHENTICATOR_H
