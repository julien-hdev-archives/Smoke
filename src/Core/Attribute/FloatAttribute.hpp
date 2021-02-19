#pragma once

#include "Attribute.hpp"

/// \brief Attribute of type Float. Basic range is [0, 1] with a step of 0.01.
/// Must be used on QML Side with a Slider.
class FloatAttribute : public Attribute
{
    Q_OBJECT
    Q_PROPERTY(float from MEMBER _from NOTIFY fromChanged)
    Q_PROPERTY(float to MEMBER _to NOTIFY toChanged)
    Q_PROPERTY(float step MEMBER _step NOTIFY stepChanged)
  public:
    // ==============================
    // Constructors & Destructor.
    // ==============================

    explicit FloatAttribute(const QString &name, const QString &label,
                            const float value, const float from = 0.f,
                            const float to = 1.f, const float step = 0.01f,
                            QObject *parent = nullptr)
        : Attribute(attributeType(), name, label, value, parent), _from(from),
          _to(to), _step(step)
    {}
    ~FloatAttribute() = default;

    // ==============================
    // Overriding Pure Virtual methods.
    // ==============================

    inline const Type
    attributeType() const override
    {
        return Type::Float;
    };

  public:
    Q_SIGNAL void fromChanged();
    Q_SIGNAL void toChanged();
    Q_SIGNAL void stepChanged();

  private:
    float _from; ///< Lowest value possible.
    float _to;   ///< Highest value possible.
    float _step; ///< Step size between values.
};