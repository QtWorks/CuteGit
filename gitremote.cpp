#include "gitremote.h"
#include <gitauthenticator.h>
#include <git2/remote.h>

GitRemote::GitRemote(git_remote* raw, GitRepository* parent) : GitBase(raw, parent)
{

}

GitRemote* GitRemote::fromName(const QString& remoteName, GitRepository* parent)
{
    git_remote* remote = nullptr;
    git_remote_lookup(&remote, parent->raw(), remoteName.toUtf8().data());
    return new GitRemote(remote, parent);
}

QString GitRemote::name() const
{
    return QString(git_remote_name(raw()));
}

void GitRemote::fetch()
{
    QString reflogRecord = QString("Fetch from remote: " + name());
    git_fetch_options opts = GIT_FETCH_OPTIONS_INIT;
    opts.callbacks.update_tips = [](const char *refname, const git_oid *a, const git_oid *b, void *data) -> int { return 0; };
    opts.callbacks.sideband_progress = [](const char *str, int len, void *payload) -> int { return 0; };
    opts.callbacks.transfer_progress = [](const git_transfer_progress *stats, void *payload) -> int {
        qDebug() << "transferProgress: " << stats->total_objects << "/" << stats->received_objects;
        return 0;
    };
    opts.callbacks.credentials = [](git_cred **cred, const char *url, const char *username_from_url, unsigned int allowed_types, void *payload) -> int {
        qDebug() << "Url: " << url;
        qDebug() << "Username: " << username_from_url;
        qDebug() << "Allowed types: " << allowed_types;

        return GitAuthenticator::instance()->authenticate(reinterpret_cast<GitRemote*>(payload), cred, username_from_url, allowed_types);
    };
    opts.callbacks.payload = this;
    git_remote_fetch(m_raw, nullptr, &opts, reflogRecord.toLatin1().data());
}
