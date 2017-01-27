#include "colorhandler.h"

#include <gitbranch.h>
#include <gitrepository.h>

#include <QDateTime>

ColorHandler::ColorHandler()
{
    qsrand(QDateTime::currentMSecsSinceEpoch());

    for(int i = 0; i < m_colors.size(); i++) {
    }
}

QString ColorHandler::color(const GitOid& oid)
{
    return m_colors.value(oid);
}

void ColorHandler::updateColors(GitRepository* repo)
{
    int red = 0;
    int green = 0;
    int blue = 0;
    BranchContainer &branches = repo->branches();

    foreach(GitBranch* branch, branches) {
        red = qrand() % 205 + 50;
        green = qrand() % 205 + 50;
        blue = qrand() % 205 + 50;
        m_colors[branch->oid()] = QString::number(red, 16) + QString::number(green, 16) + QString::number(blue, 16);
    }

    red = qrand() % 205 + 50;
    green = qrand() % 205 + 50;
    blue = qrand() % 205 + 50;
    m_colors[repo->head()] = QString::number(red, 16) + QString::number(green, 16) + QString::number(blue, 16);
}
