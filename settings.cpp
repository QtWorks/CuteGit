#include "settings.h"

#include "gitrepository.h"
#include <QFileInfo>
#include <QCryptographicHash>

namespace {
    const QString GeneralGroupKey = "General";
    const QString RepoNameKey = "Name";
    const QString RepoPathKey = "Path";
    const QString LastRepoKey = "LastRepo";
}

Settings::Settings() : QObject()
  ,m_settings("org.semlanik",
              "CuteGit")
{
}

void Settings::load(QStringList& repos)
{
    QStringList groups = m_settings.childGroups();

    foreach (QString group, groups) {
        m_settings.beginGroup(group);
        QString path = m_settings.value(RepoPathKey).toString();
        QFileInfo fileInfo(path);
        if(!fileInfo.exists(path) || !fileInfo.isDir()) {
            m_settings.remove("");
        } else {
            repos.append(path);
        }
        m_settings.endGroup();
    }
}

void Settings::add(GitRepository* repo)
{
    if(repo == nullptr) {
        return;
    }

    m_settings.beginGroup(repo->id());
    m_settings.setValue(RepoNameKey, repo->name());
    m_settings.setValue(RepoPathKey, repo->path());
    m_settings.endGroup();
}

void Settings::saveLastRepo(GitRepository* repo)
{
    m_settings.beginGroup(GeneralGroupKey);
    m_settings.setValue(LastRepoKey, repo->id());
    m_settings.endGroup();
}

QString Settings::loadLastRepo()
{
    m_settings.beginGroup(GeneralGroupKey);
    QString result = m_settings.value(LastRepoKey).toString();
    m_settings.endGroup();

    return result;
}
