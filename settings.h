#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

class GitRepository;

class Settings Q_DECL_FINAL : public QObject
{
    Q_OBJECT
public:
    static Settings* instance() {
        static Settings settings;
        return &settings;
    }

    void load(QStringList& repos);
    void add(GitRepository* repo);

    void saveLastRepo(GitRepository* activeRepoId);
    QString loadLastRepo();

private:
    Settings();
    ~Settings(){}
    Q_DISABLE_COPY(Settings)

    QSettings m_settings;
};

#endif // SETTINGS_H
