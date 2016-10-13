#ifndef COMMITVIEWMODEL_H
#define COMMITVIEWMODEL_H

#include <QObject>

class CommitGraph;

class CommitViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int x READ x CONSTANT)
    Q_PROPERTY(int y READ y CONSTANT)
    Q_PROPERTY(QString color READ color CONSTANT)

public:
    CommitViewModel(CommitGraph* graph);

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

private:
    int m_x;
    int m_y;
    QString m_color;
};

#endif // COMMITVIEWMODEL_H
