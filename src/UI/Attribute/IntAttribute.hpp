#pragma once

#include "Attribute.hpp"

/// \brief Attribute of type Int. Basic range is [0, 100] with a step of 1.
/// Must be used on QML Side with a Slider.
class IntAttribute : public Attribute
{
    Q_OBJECT
    Q_PROPERTY(int from MEMBER _from NOTIFY fromChanged)
    Q_PROPERTY(int to MEMBER _to NOTIFY toChanged)
    Q_PROPERTY(int step MEMBER _step NOTIFY stepChanged)
  public:
    // ==============================
    // Constructors & Destructor.
    // ==============================

    explicit IntAttribute(const QString &name, const QString &label,
                          const int value, const int from = 0,
                          const int to = 100, const int step = 1,
                          QObject *parent = nullptr)
        : Attribute(attributeType(), name, label, value, parent), _from(from),
          _to(to), _step(step)
    {}
    ~IntAttribute() = default;

    // ==============================
    // Overriding Virtual methods.
    // ==============================

    inline const Type
    attributeType() const override
    {
        return Type::Int;
    };

    void
    setValue(const QVariant &value) override
    {
        if (_value.toInt() == value.toInt()) return;
        _value = value;
        Q_EMIT valueChanged();
    }

  public:
    Q_SIGNAL void fromChanged();
    Q_SIGNAL void toChanged();
    Q_SIGNAL void stepChanged();

  private:
    float _from; ///< Lowest value possible.
    float _to;   ///< Highest value possible.
    float _step; ///< Step size between values.
};