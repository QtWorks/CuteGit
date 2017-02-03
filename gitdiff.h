#ifndef GITDIFF_H
#define GITDIFF_H

#include <QObject>
#include <QMap>
#include <QString>

#include <gitbase.h>

struct git_commit;

class GitRepository;
class GitCommit;
class DiffModel;
struct git_diff;

class GitDiff : public GitBase<git_diff>
{
    Q_OBJECT
    Q_PROPERTY(QStringList files READ files CONSTANT)

public:
    static GitDiff* diff(GitCommit* a, GitCommit* b);
    static GitDiff* diff(GitCommit* a);

    ~GitDiff();
    void reset();

    Q_INVOKABLE DiffModel* model(const QString& file);

    QStringList files();

private:
    GitDiff(git_diff* raw, GitRepository* repository);

    void readBody(git_diff* diff);

    QMap<QString, QPointer<DiffModel> > m_diffList;
};

#endif // GITDIFF_H
