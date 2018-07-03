#pragma once

#include <universallistmodelbase.h>

#include <QObject>
#include <QList>
#include <QHash>
#include <QPointer>
#include <QMetaProperty>
#include <QMetaObject>

#include <QDebug>


/*!
 * \brief Universal list model is QObject-base list model abstraction.
 * It exposes all objects properties as data-roles.
 */
template <typename T>
class UniversalListModel : public UniversalListModelBase
{
public:
    UniversalListModel(QObject* parent = 0) : UniversalListModelBase(parent) {}
    ~UniversalListModel() {
        clear();
    }

    int rowCount(const QModelIndex &parent) const override {
        Q_UNUSED(parent)
        return count();
    }

    int count() const override {
        return m_container.count();
    }

    QHash<int, QByteArray> roleNames() const override {
        if(s_roleNames.isEmpty()) {
            int propertyCount = T::staticMetaObject.propertyCount();
            for(int i = 1; i < propertyCount; i++) {
                s_roleNames.insert(Qt::UserRole + i, T::staticMetaObject.property(i).name());
            }
            s_roleNames[propertyCount] = "modelData";
        }
        return s_roleNames;
    }

    QVariant data(const QModelIndex &index, int role) const override
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

    /*!
     * \brief append
     * \param value
     * \return
     */
    int append(T* value) {
        Q_ASSERT_X(value != nullptr, fullTemplateName(), "Trying to add member of NULL");

        if(m_container.indexOf(value) >= 0) {
#ifdef DEBUG
            qDebug() << fullTemplateName() << "Member already exists";
#endif
            return -1;
        }
        beginInsertRows(QModelIndex(), m_container.count(), m_container.count());
        m_container.append(QPointer<T>(value));
        emit countChanged();
        endInsertRows();
        return m_container.count() - 1;
    }

    /*!
     * \brief prepend
     * \param value
     * \return
     */
    int prepend(T* value) {
        Q_ASSERT_X(value != nullptr, fullTemplateName(), "Trying to add member of NULL");

        if(m_container.indexOf(value) >= 0) {
#ifdef DEBUG
            qDebug() << fullTemplateName() << "Member already exists";
#endif
            return -1;
        }
        beginInsertRows(QModelIndex(), 0, 0);
        m_container.prepend(QPointer<T>(value));
        emit countChanged();
        endInsertRows();
        return 0;
    }

    /*!
     * \brief remove
     * \param value
     */
    void remove(T* value) {
        Q_ASSERT_X(value != nullptr, fullTemplateName(), ": Trying to remove member of NULL");

        int valueIndex = m_container.indexOf(value);

        if(valueIndex >= 0) {
            beginRemoveRows(QModelIndex(), valueIndex, valueIndex);
            m_container.removeAt(valueIndex);
            emit countChanged();
            endRemoveRows();
        }
    }

    /*!
     * \brief Resets container with new container passed as parameter
     * \param container a data for model. Should contain QPointer's to objects.
     * Passing empty container makes model empty. This method should be used to cleanup model.
     */
    void reset(const QList<QPointer<T> >& container) {
        beginResetModel();
        clear();
        m_container = container;
        emit countChanged();
        endResetModel();
    }

    /*!
     * \brief Returns the item at index position i in the list. i must be a valid index position in the list (i.e., 0 <= i < rowCount()).
     * This function is very fast (constant time).
     * \param i index of looking object
     * \return Object at provided index
     */
    T* at(int i) const {
        return m_container.at(i);
    }

    /*!
     * \brief Looking for index of objec
     * \param value
     * \return
     */
    int indexOf(T* value) const {
        return m_container.indexOf(value);
    }

    /*!
     * \brief findByProperty method finds item in internal container property of that is provided value
     * \param propertyName Latin1 name of looking property
     * \param value property of corresponded type inside QVariant container
     * \return
     */
    QPointer<T> findByProperty(const char* propertyName, const QVariant& value) const {
        auto iter = std::find_if(m_container.begin(), m_container.end(), [=](const QPointer<T> &item) -> bool {
            return item->property(propertyName) == value;
        });

        if(iter != m_container.end()) {
            return *iter;
        }
        return QPointer<T>();
    }

    /*!
     * \brief container returns internal container
     * \return
     */
    QList<QPointer<T> > container() const {
        return m_container;
    }

protected:
    void clear() {
        m_container.clear();
        emit countChanged();
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
