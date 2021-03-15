#include "Attribute.hpp"
#include "AttributeListModel.hpp"

Attribute::Attribute(const Type type, const QString &name,
                     const QString &label, const QVariant &value,
                     QObject *parent)
    : QObject(parent), _type(type), _name(name), _label(label), _value(value)
{
    if (parent)
    {
        auto attributes = qobject_cast<AttributeListModel *>(parent);
        if (!attributes)
        {
            throw "Attribute's parent must be an AttributeListModel instance.";
        }

        connect(this, &Attribute::valueChanged, attributes,
                &AttributeListModel::valueChanged);
    }
}
