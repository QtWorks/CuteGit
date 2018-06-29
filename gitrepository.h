#ifndef GITREPOSITORY_H
#define GITREPOSITORY_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QPointer>

#include <gitoid.h>

struct git_repository;

class GitBranch;
class GitTag;
class GitDiff;
class GitCommit;
class GitRemote;
struct git_oid;

using BranchContainer = QMap<QString, QPointer<GitBranch> >;
using TagContainer = QMap<GitOid, QPointer<GitTag> >;
using RemoteContainer = QMap<QString, QPointer<GitRemote> >;

class GitRepository : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString root READ root WRITE setRoot NOTIFY rootChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString path READ path NOTIFY rootChanged)
    Q_PROPERTY(GitOid head READ head WRITE setHead NOTIFY headChanged)
    Q_PROPERTY(QString id READ id NOTIFY rootChanged)

public:
    GitRepository(const QString &root);
    ~GitRepository();

    QString root() const {
        return m_root;
    }

    QString name() const {
        return m_name;
    }

    QString path() const {
        return m_path;
    }

    git_repository* raw() const {
        return m_raw;
    }

    bool isValid() const {
        return m_raw != nullptr;
    }

    BranchContainer& branches() {
        return m_branches;
    }

    TagContainer& tags() {
        return m_tags;
    }

    RemoteContainer& remotes() {
        return m_remotes;
    }

    GitOid head() const
    {
        return m_head;
    }

    QString id() const;

    Q_INVOKABLE bool isHead(const GitOid& oid) const {
        return m_head == oid;
    }

    Q_INVOKABLE void checkout(QObject* object);


public slots:
    void setRoot(QString root) {
        if (m_root == root)
            return;

        m_root = root;
        emit rootChanged(root);
    }

    void setName(QString name) {
        if (m_name == name)
            return;

        m_name = name;
        emit nameChanged(name);
    }

    void setHead(GitOid head)
    {
        if (m_head == head)
            return;

        m_head = head;
        emit headChanged(head);
    }

    void updateHead();
    void readBranches();
    void readTags();
    void readRemotes();

signals:
    void rootChanged(QString root);
    void nameChanged(QString name);

    void headChanged(GitOid head);
    void branchesChanged();

private:
    void close();

    QString m_root;
    QString m_name;
    QString m_path;
    git_repository* m_raw;

    BranchContainer m_branches;
    TagContainer m_tags;
    RemoteContainer m_remotes;
    GitOid m_head;
};

#endif // GITREPOSITORY_H
