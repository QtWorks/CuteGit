#ifndef UNIVERSALLISTMODEL_H
#define UNIVERSALLISTMODEL_H

#include <QAbstractListModel>

#include <QObject>
#include <QList>
#include <QHash>
#include <QPointer>
#include <QMetaProperty>
#include <QMetaObject>

#include <QDebug>

template <typename T>
class UniversalListModel : public QAbstractListModel
{
public:
    UniversalListModel(QObject* parent = 0) : QAbstractListModel(parent) {}
    ~UniversalListModel() {
        clear();
    }

    int rowCount(const QModelIndex &parent) const {
        Q_UNUSED(parent)
        return m_container.count();
    }

    QHash<int, QByteArray> roleNames() const {
        if(s_roleNames.isEmpty()) {
            int propertyCount = T::staticMetaObject.propertyCount();
            for(int i = 1; i < propertyCount; i++) {
                s_roleNames.insert(Qt::UserRole + i, T::staticMetaObject.property(i).name());
            }
            s_roleNames[propertyCount] = "modelData";
        }
        return s_roleNames;
    }

    QVariant data(const QModelIndex &index, int role) const
    {
        int row = index.row();

        if(row < 0 || row >= m_container.count() || m_container.at(row).isNull()) {
            return QVariant();
        }

        T* dataPtr = m_container.at(row).data();

        if(s_roleNames.value(role) == "modelData") {
            return QVariant::fromValue(dataPtr);
        }
        return dataPtr->property(s_roleNames.value(role));
    }

    bool add(T* value) {
        Q_ASSERT_X(value != nullptr, fullTemplateName(), "Trying to add member of NULL");

        if(m_container.indexOf(value) >= 0) {
#ifdef DEBUG
            qDebug() << fullTemplateName() << "Member already exists";
#endif
            return false;
        }
        beginInsertRows(QModelIndex(), m_container.count(), m_container.count());
        m_container.append(QPointer<T>(value));
        endInsertRows();
        return true;
    }

    void remove(T* value) {
        Q_ASSERT_X(value != nullptr, fullTemplateName(), ": Trying to remove member of NULL");

        int valueIndex = m_container.indexOf(value);

        if(valueIndex >= 0) {
            beginRemoveRows(QModelIndex(), valueIndex, valueIndex);
            m_container.removeAt(valueIndex);
            endRemoveRows();
        }
    }

    void reset(const QList<QPointer<T> >& container) {
        beginResetModel();
        clear();
        m_container = container;
        endResetModel();
    }

protected:
    void clear() {
        foreach (const QPointer<T>& value, m_container) {
            if(!value.isNull()) {
                delete value.data();
            }
        }
        m_container.clear();
    }

    QList<QPointer<T> > m_container;
    static QHash<int, QByteArray> s_roleNames;


private:
    static QByteArray fullTemplateName() { //Debug helper
        return QString("UniversalListModel<%1>").arg(T::staticMetaObject.className()).toLatin1();
    }
};

template<typename T>
QHash<int, QByteArray> UniversalListModel<T>::s_roleNames;

#endif // UNIVERSALLISTMODEL_H
