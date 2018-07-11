#pragma once

#include <QObject>

#include <notification.h>

class NotificationCenter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Notification* activeNotification READ activeNotification WRITE setActiveNotification NOTIFY activeNotificationChanged)
public:

    static NotificationCenter *instance() {
        static NotificationCenter _instance;
        return &_instance;
    }

    Notification *activeNotification() const
    {
        return m_activeNotification;
    }

public slots:

    void setActiveNotification(Notification *activeNotification)
    {
        if (m_activeNotification == activeNotification)
            return;

        m_activeNotification = activeNotification;
        emit activeNotificationChanged(m_activeNotification);
    }

signals:
    void activeNotificationChanged(Notification *activeNotification);

private:
    NotificationCenter();
    Notification *m_activeNotification;
};
