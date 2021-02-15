#include "AttributeListModel.hpp"
#include "Attribute.hpp"

void
AttributeListModel::append(QObject *obj)
{
    throw new std::exception("AttributeListModel should only call the "
                             "overload: append(Attribute*).");
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