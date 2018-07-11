#pragma once

#include <QObject>

class Notification : public QObject
{
    Q_PROPERTY(NotificationType type READ type WRITE setType NOTIFY typeChanged)
    Q_OBJECT
public:
    enum NotificationType {
        Stackable,
        Progress,
        BackgroundProgress,
        WarningLog
    };

    explicit Notification(QObject *parent = nullptr);

    NotificationType type() const
    {
        return m_type;
    }

signals:
    void typeChanged(NotificationType type);

public slots:
    void setType(NotificationType type)
    {
        if (m_type == type)
            return;

        m_type = type;
        emit typeChanged(m_type);
    }

private:
    NotificationType m_type;
};
