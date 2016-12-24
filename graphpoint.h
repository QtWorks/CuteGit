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
    Q_PROPERTY(GitOid oid READ oid CONSTANT)

public:
    GraphPoint(const GitOid& commitOid, QObject* parent = 0);
    GraphPoint(const GitOid& commitOid, int x, int y, const QString& color, QObject* parent = 0);
    ~GraphPoint();

    int x() const {
        return m_x;
    }

    int y() const {
        return m_y;
    }

    QString color() const {
        return m_color;
    }

    const GitOid& oid() const {
        return m_commitOid;
    }

    QString sha1() const {
        return m_commitOid.toString();
    }

    int childPointsCount() const {
        return m_childPoints.count();
    }

    QList<QObject*> childPoints() const {
        return m_childPoints;
    }

    bool addChildPoint(GraphPoint* point);

public slots:
    void setX(int x);
    void setY(int y);
    void setColor(const QString& color);

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
};

#endif // GRAPTHPOINT_H
