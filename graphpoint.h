#ifndef GRAPHPOINT_H
#define GRAPHPOINT_H

#include <QList>
#include <QObject>
#include <gitoid.h>

class GraphPoint : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(int y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(QString color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QString sha1 READ sha1 CONSTANT)
    Q_PROPERTY(QList<QObject*> childPoints READ childPoints CONSTANT)
    Q_PROPERTY(QString tag READ tag CONSTANT)
    Q_PROPERTY(QString branch READ branch CONSTANT)

public:
    GraphPoint(const GitOid& commitOid, QObject* parent = 0);
    GraphPoint(const GitOid& commitOid, int x, int y, const QString& color, QObject* parent = 0);
    ~GraphPoint();

    int x() const
    {
        return m_x;
    }

    int y() const
    {
        return m_y;
    }

    QString color() const
    {
        return m_color;
    }

    const GitOid& oid() const
    {
        return m_commitOid;
    }

    QString sha1() const
    {
        return m_commitOid.toString();
    }

    int childPointsCount() const
    {
        return m_childPoints.count();
    }

    bool addChildPoint(GraphPoint* point);

    QList<QObject*> childPoints() const
    {
        return m_childPoints;
    }

    QString tag() const
    {
        return m_tag;
    }

    QString branch() const
    {
        return m_branch;
    }

public slots:
    void setX(int x);
    void setY(int y);
    void setColor(const QString& color);

    void setTag(QString tag)
    {
        m_tag = tag;
    }

    void setBranch(QString branch)
    {
        m_branch = branch;
    }

signals:
    void xChanged(int x);
    void yChanged(int y);
    void colorChanged(const QString& color);

private:
    GitOid m_commitOid;
    QList<QObject*> m_childPoints;

    int m_x;
    int m_y;
    QString m_color;
    QString m_tag;
    QString m_branch;
};

#endif // GRAPTHPOINT_H
