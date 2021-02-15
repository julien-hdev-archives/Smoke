#pragma once

#include "Utility/ObjectListModel.hpp"

class Attribute;

class AttributeListModel : public ObjectListModel
{
    Q_OBJECT
  public:
    explicit AttributeListModel(QObject *parent = nullptr)
        : ObjectListModel(parent)
    {}
    ~AttributeListModel() = default;

    void append(QObject *obj) override;

    /// \brief Add the Attribute inside the list, if not already in it.
    /// \param attr Pointer to the Attribute.
    void append(Attribute *attr);

    /// \brief Find an Attribute by its name.
    /// \param name Name of the Attribute.
    /// \returns Attribute or nullptr.
    const Attribute *find(const QString &name) const;
};