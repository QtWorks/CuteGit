#include "gitrepository.h"

#include <QDebug>
#include <QFileInfo>
#include <QDir>

#include <git2.h>

GitRepository::GitRepository(const QString& root) : QObject()
  ,m_repo(nullptr)
{
    if(git_repository_open(&m_repo, root.toUtf8().data()) != 0) {
        qDebug() << "Cannot open repository";
        close();
        return;
    }

    QFileInfo info(root);
    m_root = root;
    m_name = info.dirName();
    m_path = info.filePath();
    qDebug() << "New repo:" << m_name << m_root << m_path;
}

GitRepository::~GitRepository()
{
    close();
}

void GitRepository::close()
{
    if(m_repo) {
        git_repository_free(m_repo);
    }
    m_repo = nullptr;
}
