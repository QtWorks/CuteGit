#ifndef GITREPOSITORY_H
#define GITREPOSITORY_H

#include <QObject>
#include <QString>

extern "C"
{
    struct git_repository;
}


class GitRepository : public QObject
{
    Q_PROPERTY(QString root READ root WRITE setRoot NOTIFY rootChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    GitRepository();
    ~GitRepository();

    Q_INVOKABLE void open(const QString &path);
    Q_INVOKABLE void open(const QUrl &url);
    Q_INVOKABLE void close();

    QString root() const {
        return m_root;
    }

    QString name() const {
        return m_name;
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
    QString m_root;
    QString m_name;
    git_repository* m_repo;
};

#endif // GITREPOSITORY_H
