#ifndef GITDIFF_H
#define GITDIFF_H

#include <QObject>
#include <QMap>
#include <QString>

#include <diffmodel.h>

struct git_commit;

class GitRepository;

class GitDiff : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList files READ files CONSTANT)

public:
    GitDiff(git_commit* a, git_commit* b, GitRepository* repository);
    ~GitDiff();
    void readBody(git_commit* a, git_commit* b);
    void reset();

    Q_INVOKABLE DiffModel* model(const QString& file);

    QStringList files();

private:
    QMap<QString, QPointer<DiffModel> > m_diffList;
    GitRepository* m_repository;
};

#endif // GITDIFF_H
