#ifndef GITDIFF_H
#define GITDIFF_H

#include <QObject>
#include <QHash>
#include <QString>

struct git_commit;

class GitRepository;

class GitDiff : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList files READ files CONSTANT)

public:
    GitDiff(git_commit* a, git_commit* b, GitRepository* repository);
    ~GitDiff(){}
    void readBody(git_commit* a, git_commit* b);
    void reset();

    Q_INVOKABLE QString unified(const QString& file);

    QStringList files();

private:
    QHash<QString, QString> m_diffList;
    GitRepository* m_repository;
};

#endif // GITDIFF_H
