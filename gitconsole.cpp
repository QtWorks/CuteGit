#include "gitconsole.h"

#include <QProcess>
#include <gitrepository.h>
#include <QTimer>

struct Autocomplete {
    Autocomplete() {
        m_gitCommands
                      << "git add"
                      << "git mv"
                      << "git reset"
                      << "git rm"
                      << "git grep"
                      << "git log"
                      << "git show"
                      << "git status"
                      << "git branch"
                      << "git checkout"
                      << "git commit"
                      << "git diff"
                      << "git rebase"
                      << "git tag"
                      << "git fetch"
                      << "git pull"
                      << "git clean"
                      << "git reflog"
                      << "git push"
                      << "git submodule"
                      << "git cherry-pick"
                      << "git revert"
                      << "git remote"
                      << "git config"
                      << "git clean"
                      << "git stash";
        qSort(m_gitCommands);
    }

    QList<QString> listMatches(const QString& text, QString& commonPart) {
        QList<QString> matches;
        foreach(QString command, m_gitCommands) {
            if(command.startsWith(text)) {
                matches.append(command);
            }
        }

        if(matches.count() > 1) {
            int i = 0;
            for(; i < qMax(matches.first().length(), matches.last().length()); i++) {
                if(matches.first().at(i) != matches.last().at(i)) {
                    break;
                }
            }
            commonPart = matches.first().mid(0, i);
        }
        return matches;
    }

    QList<QString> m_gitCommands;
};

namespace  {
    Autocomplete completer;
}

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
        emit commandLog(QString("<b>$&nbsp;") + command.toHtmlEscaped() + QString("</b><br/>"), false);
        emit commandLog(tr("Only <b>git</b> command is accetable").append("<br/>"), true);
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
    QByteArray log = m_process->readAllStandardOutput();
    log += m_process->readAllStandardError();

    emit commandLog(QString::fromUtf8(log).toHtmlEscaped().replace("\n","<br/>"), true);
    setBusy(false);
}

void GitConsole::onOutputReady()
{
//    QByteArray log = m_process->readAll();
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

void GitConsole::requestAutocomplete(const QString& current)
{
    QString commonPart;
    QList<QString> matches = completer.listMatches(current.trimmed(), commonPart);

    if(matches.count() == 1) {
        emit autocomplete(matches.first());
        return;
    }

    if(matches.count() <= 0) {
        return;
    }

    emit autocomplete(commonPart);
}

void GitConsole::requestPredictions(QString current)
{
    QString commonPart;
    current = current.trimmed();

    QList<QString> matches = completer.listMatches(current, commonPart);
    if((!current.isEmpty() && matches.count() > 1) || (matches.count() == 1 && current != matches.first())) {
        emit predict(completer.listMatches(current, commonPart));
        return;
    }
    emit predict(QList<QString>());
}
