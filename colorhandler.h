#ifndef COLORHANDLER_H
#define COLORHANDLER_H

#include <QString>
#include <QMap>

#include <gitoid.h>

class ColorHandler
{

public:
    static ColorHandler& instance() {
        static ColorHandler _instance;
        return _instance;
    }

    void updateColors(GitRepository* repo);
    QString color(const GitOid& oid);

private:
    ColorHandler();
    Q_DISABLE_COPY(ColorHandler)

    QMap<GitOid, QString> m_colors;
};

#endif // COLORHANDLER_H
