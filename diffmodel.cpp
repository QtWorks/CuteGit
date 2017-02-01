#include "diffmodel.h"

DiffModel::DiffModel(const QString &path, QObject *parent) : QObject(parent)
  ,m_path(path)
  ,m_type(Unmodified)
{

}

DiffModel::~DiffModel()
{

}

DiffModel::DiffModel(const DiffModel& other) : QObject()
{
    m_type = other.m_type;
    m_path = other.m_path;
    m_data = other.m_data;
}

DiffModel& DiffModel::operator=(const DiffModel& other)
{
    if(this != &other) {
        m_type = other.m_type;
        emit typeChanged(m_type);
        m_path = other.m_path;
        m_data = other.m_data;
        emit dataChanged(m_data);
    }

    return *this;
}

void DiffModel::append(const QString& unifiedData)
{
    m_data.append(unifiedData);
    emit dataChanged(m_data);
}

