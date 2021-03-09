#pragma once

#include "UI/Attribute/Attribute.hpp"

#include <QUndoCommand>

/// \brief Undo Stack command used to set the value of an attribute.
class SetAttributeCommand : public QUndoCommand
{
  public:
    explicit SetAttributeCommand(Attribute *attribute, const QVariant &value)
        : QUndoCommand("Change attribute value."), _attribute(attribute),
          _oldValue(attribute->getValue()), _value(value)
    {}
    ~SetAttributeCommand() = default;

    void
    undo() override
    {
        _attribute->setValue(_oldValue);
    }

    void
    redo() override
    {
        _attribute->setValue(_value);
    }

  private:
    Attribute *_attribute;
    QVariant _oldValue;
    QVariant _value;
};