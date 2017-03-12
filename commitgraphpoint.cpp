#include "commitgraphpoint.h"

CommitGraphPoint::CommitGraphPoint(const GitOid& commitOid, QObject* parent) : GraphPoint(parent)
    ,m_commitOid(commitOid)
{

}

CommitGraphPoint::CommitGraphPoint(const GitOid &commitOid, int x, int y, const QString &color, QObject *parent) : GraphPoint(x, y, color, parent)
  ,m_commitOid(commitOid)
{

}

CommitGraphPoint::~CommitGraphPoint()
{

}
