#pragma once

#include "UI/Attribute/AttributeListModel.hpp"
#include "UI/Attribute/FloatAttribute.hpp"

#include <QObject>

struct Simulator_Params
{
    float haveToReset = 0.f;
    float radius = 1.0f;
    float viscosity = 0.4f;
    float pA = 2.0f;
};

class SimulatorProperties : public QObject
{
    Q_OBJECT
    Q_PROPERTY(AttributeListModel *attributes MEMBER _attributes NOTIFY
                   attributesChanged)
  public:
    SimulatorProperties(QObject *parent);
    ~SimulatorProperties() = default;

    /// \brief Create a Simulator_Params,
    /// used for the Rendering thread.
    const Simulator_Params simulator_Params() const;

    Attribute *
    findAttribute(const QString &name) const
    {
        return _attributes->find(name);
    }

  public:
    Q_SIGNAL void attributesChanged();
    Q_SIGNAL void valueChanged();

  private:
    AttributeListModel *_attributes;
};