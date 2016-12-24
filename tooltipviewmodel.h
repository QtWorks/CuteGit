#ifndef TOOLTIPVIEWMODEL_H
#define TOOLTIPVIEWMODEL_H

#include <QObject>

class QQuickItem;

class TooltipViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(int x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(int y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(QQuickItem* viewport READ viewport WRITE setViewport NOTIFY viewportChanged)
public:
    TooltipViewModel();

    int x() const
    {
        return m_x;
    }

    int y() const
    {
        return m_y;
    }

    bool visible() const
    {
        return m_visible;
    }

    QString text() const
    {
        return m_text;
    }

    QQuickItem* viewport() const
    {
        return m_viewport;
    }

public slots:
    void setX(int x)
    {
        if (m_x == x)
            return;

        m_x = x;
        emit xChanged(x);
    }

    void setY(int y)
    {
        if (m_y == y)
            return;

        m_y = y;
        emit yChanged(y);
    }

    void setVisible(bool visible)
    {
        if (m_visible == visible)
            return;

        m_visible = visible;
        emit visibleChanged(visible);
    }

    void setText(const QString& text)
    {
        if (m_text == text)
            return;

        m_text = text;
        emit textChanged(text);
    }

    void setViewport(QQuickItem* viewport)
    {
        if (m_viewport == viewport)
            return;

        m_viewport = viewport;
        emit viewportChanged(viewport);
    }

signals:
    void xChanged(int x);

    void yChanged(int y);

    void visibleChanged(bool visible);

    void textChanged(const QString& text);

    void viewportChanged(QQuickItem* viewport);

private:
    int m_x;
    int m_y;
    bool m_visible;
    QString m_text;
    QQuickItem* m_viewport;
};

#endif // TOOLTIPVIEWMODEL_H
