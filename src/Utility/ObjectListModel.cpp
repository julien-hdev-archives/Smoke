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
    return QVariant::fromValue<QObject *>(obj);
}

Qt::ItemFlags
ObjectListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags f = QAbstractListModel::flags(index);
    if (index.isValid()) f |= Qt::ItemIsEditable;
    return f;
}

void
ObjectListModel::populate(const QObjectList &objects)
{
    beginResetModel();
    _data = objects;
    endResetModel();
}