#ifndef DIFFMODEL_H
#define DIFFMODEL_H

#include <QObject>
#include <QString>

#include <git2/diff.h>

class DiffModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString path READ path CONSTANT)
    Q_PROPERTY(QString data READ data NOTIFY dataChanged)
    Q_PROPERTY(int type READ type NOTIFY typeChanged)
    Q_PROPERTY(quint16 similarity READ similarity NOTIFY similarityChanged)
    Q_ENUMS(DiffType)
public:
    enum DiffType {
        Unmodified = GIT_DELTA_UNMODIFIED,
        Added = GIT_DELTA_ADDED,
        Deleted = GIT_DELTA_DELETED,
        Modified = GIT_DELTA_MODIFIED,
        Renamed = GIT_DELTA_RENAMED,
        Copied = GIT_DELTA_COPIED,
        Ignored = GIT_DELTA_IGNORED,
        Untracked = GIT_DELTA_UNTRACKED,
        TypeChange = GIT_DELTA_TYPECHANGE,
        Unreadable = GIT_DELTA_UNREADABLE,
        Conflicted = GIT_DELTA_CONFLICTED
    };

    explicit DiffModel(const QString& path = QString(), QObject *parent = 0);
    DiffModel(const DiffModel& other);
    DiffModel& operator=(const DiffModel& other);
    ~DiffModel();

    void append(const QString& unifiedData);

    QString data() const
    {
        return m_data;
    }

    int type() const
    {
        return m_type;
    }

    QString path() const
    {
        return m_path;
    }

    void setType(int type)
    {
        if(m_type != type) {
            m_type = type;
            emit typeChanged(m_type);
        }
    }

    quint16 similarity() const
    {
        return m_similarity;
    }

public slots:
    void setSimilarity(quint16 similarity)
    {
        if (m_similarity == similarity)
            return;

        m_similarity = similarity;
        emit similarityChanged(similarity);
    }

signals:
    void dataChanged(QString data);

    void typeChanged(int type);

    void similarityChanged(quint16 similarity);

private:
    QString m_path;
    int m_type;
    QString m_data;
    quint16 m_similarity;
};

#endif // DIFFMODEL_H
