#ifndef REPOSITORYMODEL_H
#define REPOSITORYMODEL_H

#include <universallistmodel.h>
#include <gitrepository.h>
#include <QString>
#include <QPointer>

class RepositoryModel : public UniversalListModel<GitRepository>
{
    Q_OBJECT
    Q_PROPERTY(int activeRepositoryIndex READ activeRepositoryIndex NOTIFY activeRepositoryIndexChanged)
public:
    RepositoryModel(QObject *parent = 0);
    ~RepositoryModel();
    void addRepository(GitRepository* repository);
    void removeRepository(GitRepository* repository);
    int activeRepositoryIndex() const
    {
        return m_activeRepositoryIndex;
    }

public slots:
    void setActiveRepositoryIndex(int activeRepositoryIndex)
    {
        if (m_activeRepositoryIndex == activeRepositoryIndex)
            return;

        m_activeRepositoryIndex = activeRepositoryIndex;
        emit activeRepositoryIndexChanged(activeRepositoryIndex);
    }

signals:
    void activeRepositoryIndexChanged(int activeRepositoryIndex);

private:
    QList<QString> m_repolist;
    int m_activeRepositoryIndex;
};


#endif // REPOSITORYMODEL_H
