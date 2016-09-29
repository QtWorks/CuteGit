#include "githandler.h"

#include <QDebug>
#include <QUrl>

extern "C" {
#include <git2.h>
}

GitHandler::GitHandler() : QObject()
    ,_repo(nullptr)
{
    git_libgit2_init();
}

GitHandler::~GitHandler()
{
    close();
    git_libgit2_shutdown();
}

void GitHandler::open(const QUrl &url)
{
    if(url.isLocalFile()) {
        open(url.toLocalFile());
    }
}

void GitHandler::open(const QString &path)
{
    close();

    git_buf root = {0,0,0};
    if(git_repository_discover(&root, path.toUtf8().data(), 0, NULL) != 0) {
        qDebug() << lastError();
        return;
    }

    QString str = QString::fromUtf8(root.ptr, root.size);

    if(git_repository_open(&_repo, str.toUtf8().data()) != 0) {
        qDebug() << "Cannot open repository";
        qDebug() << "Error:" << lastError();
        close();
    }
}

void GitHandler::close()
{
    if(_repo) {
        git_repository_free(_repo);
    }
    _repo = nullptr;
}

QString GitHandler::lastError() const
{
    const git_error *e = giterr_last();
    if(e) {
        return QString("(%1): %2").arg(e->klass).arg(e->message);
        giterr_clear();
    }
    giterr_clear();
    return QString();
}
