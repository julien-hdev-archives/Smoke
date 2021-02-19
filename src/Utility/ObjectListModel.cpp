#include "ObjectListModel.hpp"

int
ObjectListModel::rowCount(const QModelIndex &parent) const
{
    return _data.size();
}

QVariant
ObjectListModel::data(const QModelIndex &index, int role) const
{
    if (!hasIndex(index.row(), index.column(), index.parent())) return {};

    auto obj = _data.at(index.row());
    if (role == Qt::UserRole) return QVariant::fromValue<QObject *>(obj);

    return {};
}

Qt::ItemFlags
ObjectListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags f = QAbstractListModel::flags(index);
    if (index.isValid()) f |= Qt::ItemIsEditable;
    return f;
}

QHash<int, QByteArray>
ObjectListModel::roleNames() const
{
    QHash<int, QByteArray> res;
    // Allow to access the element in QML using the keyword 'object'.
    res[static_cast<int>(Qt::UserRole)] = "object";

    return res;
}

void
ObjectListModel::append(QObject *obj)
{
    if (_data.contains(obj)) return;

    // Make sure to take the ownership.
    obj->setParent(this);
    // Add the object in the list.
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _data.append(obj);
    endInsertRows();
}
