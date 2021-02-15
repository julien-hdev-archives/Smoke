#pragma once

#include <QObject>
#include <QVariant>

/// \brief Abstract class used for Attributes.
/// \remarks Because of the QObject inheritance, it is not possible
/// to make it templated. That is why we use a QVariant to store the value.
/// \todo Add the Component to which the Attribute belongs.
class Attribute : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Type type READ getType NOTIFY typeChanged)
    Q_PROPERTY(QString name READ getName NOTIFY nameChanged)
    Q_PROPERTY(QString label READ getLabel NOTIFY labelChanged)
    Q_PROPERTY(QVariant value READ getValue WRITE setValue NOTIFY valueChanged)

  public:
    // ==============================
    // Attribute Enum.
    // ==============================

    /// \brief Enum used to know to which type
    /// the attribute value corresponds.
    enum Type
    {
        Float
    };
    Q_ENUM(Type);

    inline const Type
    getType() const
    {
        return _type;
    }

    // ==============================
    // Constructors & Destructor.
    // ==============================

    explicit Attribute(const Type type, const QString &name,
                       const QString &label, const QVariant &value,
                       QObject *parent = nullptr)
        : QObject(parent), _type(type), _name(name), _label(label),
          _value(value)
    {}
    virtual ~Attribute() = default;

    // ==============================
    // Pure Virtual methods.
    // ==============================

    /// \brief Must be overriden in sub-classes.
    /// Used only to avoid the 'Attribute' class to be instantiated.
    virtual const Type attributeType() const = 0;

    // ==============================
    // Properties accessors.
    // ==============================

    inline const QString &
    getName() const
    {
        return _name;
    }

    inline const QString &
    getLabel() const
    {
        return _label;
    }

    inline const QVariant &
    getValue() const
    {
        return _value;
    }

    virtual void
    setValue(const QVariant &value)
    {
        if (_value == value) return;
        _value = value;
        Q_EMIT valueChanged();
    }

  public:
    Q_SIGNAL void typeChanged();
    Q_SIGNAL void nameChanged();
    Q_SIGNAL void labelChanged();
    Q_SIGNAL void valueChanged();

  protected:
    const Type _type;
    const QString _name;
    const QString _label;
    QVariant _value;
    // Add the Component to which it belongs.
};