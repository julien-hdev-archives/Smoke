#include "AttributeListModel.hpp"
#include "Attribute.hpp"

AttributeListModel::AttributeListModel(QObject *parent)
    : ObjectListModel(parent)
{
    if (parent)
    {
        auto res = connect(this, SIGNAL(valueChanged()), parent,
                           SIGNAL(valueChanged()));
        if (!static_cast<bool>(res))
        {
            throw "The connection between signals cannot be done.";
        }
    }
}

void
AttributeListModel::append(QObject *obj)
{
    throw "AttributeListModel should only call the "
        "overload: append(Attribute*).";
}

void
AttributeListModel::append(Attribute *attr)
{
    ObjectListModel::append(attr);
}

const Attribute *
AttributeListModel::find(const QString &name) const
{
    for (const auto obj : _data)
    {
        auto attr = reinterpret_cast<Attribute *>(obj);
        if (attr->getName() == name) return attr;
    }

    return nullptr;
}
