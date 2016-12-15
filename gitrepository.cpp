#include "gitrepository.h"

#include <QDebug>
#include <QFileInfo>
#include <QDir>

#include <gitbranch.h>
#include <gitcommit.h>
#include <gittag.h>
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
    m_name = m_path;//TODO: replace with Human readable name
    qDebug() << "New repo:" << m_name << m_root << m_path;
    readBranches();
    readTags();
}

GitRepository::~GitRepository()
{
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
    while(git_branch_next(&branchRef, &branchType, iter) == 0)
    {
        GitBranch* branch = new GitBranch(branchRef, branchType, this);
        m_branches.insert(branch->name(), QPointer<GitBranch>(branch));
        qDebug() << branch->name();
        qDebug() << branch->type();
    }
}

void GitRepository::readTags()
{
    git_tag_foreach(raw(),
                [](const char *name, git_oid *oid, void *payload) -> int
    {
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
