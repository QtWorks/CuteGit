#ifndef GITCONSOLE_H
#define GITCONSOLE_H

#include <QObject>
#include <QPointer>
#include <QList>
#include <QString>

class QProcess;
class GitRepository;

class GitConsole : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool busy READ busy WRITE setBusy NOTIFY busyChanged)
    Q_PROPERTY(QString recent READ recent NOTIFY recentChanged)
public:
    GitConsole(QObject* parent = 0);
    ~GitConsole();
    Q_INVOKABLE void exec(const QString& command);
    bool busy() const
    {
        return m_busy;
    }

    QString recent() const
    {
        if(m_recentIndex < 0) {
            return QString();
        }

        return m_recentContainer.at(m_recentIndex);
    }

public slots:
    void setRepository(GitRepository* repo);

    void setBusy(bool busy)
    {
        if (m_busy == busy)
            return;

        m_busy = busy;
        emit busyChanged(busy);
    }

    void onFinished(int exitCode);
    void onOutputReady();

    Q_INVOKABLE void recentUp();
    Q_INVOKABLE void recentDown();

signals:
    void busyChanged(bool busy);
    void commandLog(const QString& data, bool prepend);
    void commandError();
    void recentChanged(QString recent);

private:
    QProcess* m_process;
    QPointer<GitRepository> m_repo;
    bool m_busy;
    QList<QString> m_recentContainer;
    int m_recentIndex;
};

#endif // GITCONSOLE_H
