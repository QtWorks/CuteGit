#include "gitrepository.h"

#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QCryptographicHash>

#include <gitbranch.h>
#include <gitcommit.h>
#include <gittag.h>
#include <gitremote.h>
#include <gitdiff.h>

#include <git2.h>

GitRepository::GitRepository(const QString& root) : QObject(nullptr)
{
    if(git_repository_open(&m_raw, root.toUtf8().data()) != 0) {
        qDebug() << "Cannot open repository";
        close();
        return;
    }

    m_root = root;
    m_path = git_repository_workdir(m_raw);
    m_name = m_path.split("/", QString::SkipEmptyParts).last();
    qDebug() << "New repo:" << m_name << m_root << m_path;

    readBranches();
    readTags();
    readRemotes();
    updateHead();
}

GitRepository::~GitRepository()
{
    qDebug() << "GitRepository::~GitRepository";
    close();
}

void GitRepository::close()
{
    if(m_raw) {
        git_repository_free(m_raw);
    }
    m_raw = nullptr;
}

void GitRepository::readBranches()
{
    git_reference *branchRef;
    git_branch_t branchType;
    git_branch_iterator* iter;
    git_branch_iterator_new(&iter, m_raw, GIT_BRANCH_ALL);
    while(git_branch_next(&branchRef, &branchType, iter) == 0) {
        GitBranch* branch = new GitBranch(branchRef, branchType, this);
        m_branches.insert(branch->fullName(), QPointer<GitBranch>(branch));
        qDebug() << branch->fullName();
        qDebug() << branch->type();
    }
    emit branchesChanged();
}

void GitRepository::readTags()
{
    git_tag_foreach(raw(),
                [](const char *name, git_oid *oid, void *payload) -> int {
        Q_UNUSED(payload)
        Q_UNUSED(name)

        GitRepository* repo = static_cast<GitRepository*>(payload);
        git_tag* tagraw = 0;
        if(git_tag_lookup(&tagraw, repo->raw(), oid) != 0) {
            qCritical() << "Invalid tag found. Broken repository";
            return 1;
        }
        GitTag* tag = new GitTag(tagraw, repo);
        if(tag->isValid()) {
            repo->m_tags.insert(tag->targetId(), tag);
        }
        qDebug() << "Tag found: " << tag->name() << tag->sha1();
        return 0;
    },
    this);
}

void GitRepository::readRemotes()
{
    git_strarray str_array;
    git_remote_list(&str_array, raw());
    for(int i = 0; i < str_array.count; i++) {
        GitRemote* remote = GitRemote::fromName(QString::fromLatin1(str_array.strings[i]), this);
        if(remote) {
            m_remotes.insert(remote->name(), QPointer<GitRemote>(remote));
        }
    }
//TODO: Why this code was left?
//    git_reference_foreach_glob(raw(),"refs/remotes/*", [](const char *name, void *payload) -> int
//    {
//        qDebug() << "Remotes: " << name;
//        return 0;
////        if(git_reference_is_remote(reference)) {
////        }
//    }, this);

//    git_remote* remoteRaw;
//    git_reference_is_remote()
}

void GitRepository::checkout(QObject* object)
{
    git_checkout_options opts;
    git_checkout_init_options(&opts, GIT_CHECKOUT_OPTIONS_VERSION);

    opts.checkout_strategy = GIT_CHECKOUT_FORCE;//TODO: modify to merge policy
    opts.notify_flags = GIT_CHECKOUT_NOTIFY_ALL;
    opts.notify_cb = [](
            git_checkout_notify_t why,
            const char *path,
            const git_diff_file *baseline,
            const git_diff_file *target,
            const git_diff_file *workdir,
            void *payload) -> int {
        //TODO: make popup with progressbar
//        qDebug() << "path:" << path;
//        switch (why) {
//        case GIT_CHECKOUT_NOTIFY_CONFLICT:
//            qDebug() << "conflict";
//            break;
//        case GIT_CHECKOUT_NOTIFY_DIRTY:
//            qDebug() << "dirty";
//            break;
//        case GIT_CHECKOUT_NOTIFY_UPDATED:
//            qDebug() << "updated";
//            break;
//        case GIT_CHECKOUT_NOTIFY_UNTRACKED:
//            qDebug() << "untracked";
//            break;
//        case GIT_CHECKOUT_NOTIFY_IGNORED:
//            qDebug() << "ignored";
//            break;
//        default:
//            break;
//        }

        return 0;
    };

    GitOid oid;
    GitBranch* branch = dynamic_cast<GitBranch*>(object);
    GitCommit* commit = dynamic_cast<GitCommit*>(object);

    if(branch != nullptr) {
        commit = GitCommit::fromOid(branch->oid());
        qDebug() << "Checkout branch: " << git_checkout_tree(raw(), (git_object*)(commit->raw()), &opts);
        switch(branch->type()) {
        case GitBranch::Local:
            break;
        case GitBranch::Remote:
            git_reference* ref;
            //TODO: better to create API for local branch creation
            if(0 == git_branch_create_from_annotated(&ref, raw(), branch->name().toUtf8().data(), branch->annotatedCommit(), 0)) {
                branch = new GitBranch(ref, GIT_BRANCH_LOCAL, this);
                branch->setUpstream(*branch);
                m_branches.insert(branch->fullName(), QPointer<GitBranch>(branch));
                emit branchesChanged();
            } else if(m_branches.contains(branch->name())) { /*
                                                               TODO: need to think how valid is this.
                                                               In case if local branch already exists
                                                               it will be checked out.
                                                               But from commit tree perspective it doesn't
                                                               look as valid behavior.
                                                             */
                branch = m_branches.value(branch->name());
            }
            break;
        }

        git_repository_set_head(raw(), branch->refName().toLatin1().data());
        oid = branch->oid();
        delete commit;
    } else if(commit != nullptr) {
        qDebug() << "Checkout commit: " << git_checkout_tree(raw(), (git_object*)(commit->raw()), &opts);
        git_repository_set_head_detached(raw(), commit->oid().raw());
        oid = commit->oid();
    } else {
        qDebug() << "Invalid object for checkout";
        return;
    }

    setHead(oid);
}

void GitRepository::updateHead()
{
    //Read head
    git_reference* ref = nullptr;
    git_repository_head(&ref, raw());
    git_annotated_commit* commit = nullptr;
    git_annotated_commit_from_ref(&commit, raw(), ref);
    setHead(GitOid(git_annotated_commit_id(commit), this));
    qDebug() << "Repo head" << m_head.toString();
}

QString GitRepository::id() const
{
    return QCryptographicHash::hash(name().toUtf8() + path().toUtf8(), QCryptographicHash::Sha1).toHex();
}

#if 0 //Test functionality
void GitRepository::testReadBranches()
{
    QList<GitBranch*> m_branches;
    git_reference *branchRef;
    git_branch_t branchType;
    git_branch_iterator* iter;
    git_branch_iterator_new(&iter, m_raw, GIT_BRANCH_ALL);
    while(git_branch_next(&branchRef, &branchType, iter) == 0) {
        GitBranch* branch = new GitBranch(branchRef, branchType, this);
        qDebug() << branch->fullName();
        qDebug() << branch->type();
        GitBranch upstreamBranch = std::move(branch->upstream());
        m_branches.append(branch);
//        qDebug() << upstreamBranch.fullName();
    }

    for(auto branch : m_branches)
    {
        GitBranch branchMoved(std::move(*branch));
        delete branch;
        GitBranch upstream = branchMoved.upstream();
        qDebug() << "valid upstream:" << upstream.isValid();
    }
}
#endif
