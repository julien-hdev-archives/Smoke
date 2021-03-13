#pragma once

#include "UI/Attribute/Attribute.hpp"
#include "UI/Commands.hpp"

#include <QObject>
#include <QUndoStack>

/// \brief Manage the application.
/// Designed to be used as context property for QML.
class AppManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUndoStack *undoStack MEMBER _undoStack CONSTANT)

  public:
    explicit AppManager(QObject *parent) : QObject(parent)
    {
        _undoStack = new QUndoStack(this);
    }
    ~AppManager() = default;

    // ==============================
    // UNDOSTACK - UndoCommand methods.
    // ==============================

    /// \brief Set the value of an attribute using an UndoCommand.
    Q_SLOT
    void
    setAttributeValue(Attribute *attribute, const QVariant &value) const
    {
        // Ownership handled by the stack.
        _undoStack->push(new SetAttributeCommand(attribute, value));
    }

  private:
    QUndoStack *_undoStack;
};