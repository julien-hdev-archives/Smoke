#pragma once

#include "Core/AttributeListModel.hpp"
#include "Core/FloatAttribute.hpp"

#include <QObject>

struct SdfRenderer_Params
{
    float absorptionCoefficient = 0.5f;
    float absorptionCutoff = 0.25f;
    float marchMultiplier = 1.8f;
    int maxVolumeMarchSteps = 20;
    int maxVolumeLightMarchSteps = 4;
    int maxSdfSphereSteps = 15;
    int maxOpaqueShadowMarchSteps = 10;
};

class SdfRendererProperties : public QObject
{
    Q_OBJECT
    Q_PROPERTY(AttributeListModel *attributes MEMBER _attributes NOTIFY
                   attributesChanged)
  public:
    SdfRendererProperties(QObject *parent = nullptr) : QObject(parent)
    {
        // Construct the ListModel.
        _attributes = new AttributeListModel(this);

        // Get the default attribute values.
        SdfRenderer_Params initialParams;

        // Initialize each attribute.
        // Only one for now, for the test.
        auto absorptionCoefficient = new FloatAttribute(
            "absorptionCoefficient", "Absorption Coefficient",
            initialParams.absorptionCoefficient, 0.f, 1.f, 0.01f, _attributes);
        _attributes->append(absorptionCoefficient);
    }
    ~SdfRendererProperties() = default;

  public:
    Q_SIGNAL void attributesChanged();

  private:
    AttributeListModel *_attributes;
};