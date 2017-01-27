#include "gitconsole.h"

#include <QProcess>
#include <gitrepository.h>

GitConsole::GitConsole(QObject *parent) : QObject(parent)
  ,m_process(new QProcess(this))
  ,m_busy(false)
  ,m_recentIndex(-1)
{
    connect(m_process, &QProcess::readyRead, this, &GitConsole::onOutputReady);
    connect(m_process, SIGNAL(finished(int)), this, SLOT(onFinished(int)));
}

GitConsole::~GitConsole()
{
    if(m_process && m_process->state() != QProcess::NotRunning) {
        m_process->terminate();
    }
}

void GitConsole::exec(const QString& command)
{
    m_recentIndex = -1;
    if(!command.startsWith("git ")) {
        emit commandError();
        return;
    }
    qDebug() << "Execute:" << command << "in" << m_process->workingDirectory();
    m_process->start(command);
    emit commandLog(QString("<b>$&nbsp;") + command.toHtmlEscaped() + QString("</b><br/>"), false);
    setBusy(true);
    if(m_recentContainer.count() <= 0 || m_recentContainer.first() != command) {
        m_recentContainer.push_front(command);
    }
}

void GitConsole::setRepository(GitRepository *repo)
{
    m_repo = repo;
    m_process->setWorkingDirectory(m_repo->path());
}

void GitConsole::onFinished(int exitCode)
{
    if(exitCode != 0 ) {
        emit commandError();
    }
    setBusy(false);
}

void GitConsole::onOutputReady()
{
    QByteArray log = m_process->readAll();
    emit commandLog(QString::fromUtf8(log).toHtmlEscaped().replace("\n","<br/>"), true);
}

void GitConsole::recentUp()
{
    if(m_recentContainer.count() <= 0) {
        return;
    }

    if(m_recentIndex >= (m_recentContainer.count() - 2)) {
        m_recentIndex = m_recentContainer.count() - 2;
    }

    emit recentChanged(m_recentContainer.at(++m_recentIndex));
}

void GitConsole::recentDown()
{
    if(m_recentContainer.count() <= 0) {
        return;
    }

    if(m_recentIndex <= 0) {
        m_recentIndex = -1;
        emit recentChanged(QString());
        return;
    }
    emit recentChanged(m_recentContainer.at(--m_recentIndex));
}
