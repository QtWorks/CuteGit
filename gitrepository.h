#ifndef GITREPOSITORY_H
#define GITREPOSITORY_H

#include <QObject>
#include <QString>
#include <QHash>
#include <QPointer>

struct git_repository;

class GitBranch;
typedef QHash<QString, QPointer<GitBranch>> BranchContainer;

class GitRepository : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString root READ root WRITE setRoot NOTIFY rootChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString path READ path NOTIFY rootChanged)

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

signals:
    void rootChanged(QString root);
    void nameChanged(QString name);

private:
    void close();
    void readBranches();

    QString m_root;
    QString m_name;
    QString m_path;
    git_repository* m_raw;

    BranchContainer m_branches;
};

#endif // GITREPOSITORY_H
