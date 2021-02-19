#pragma once

#include "Core/Attribute/AttributeListModel.hpp"
#include "Core/Attribute/FloatAttribute.hpp"

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
    SdfRendererProperties(QObject *parent);
    ~SdfRendererProperties() = default;

    /// \brief Create a SdfRenderer_Params,
    /// used for the Rendering thread.
    const SdfRenderer_Params sdfRenderer_Params() const;

  public:
    Q_SIGNAL void attributesChanged();
    Q_SIGNAL void valueChanged();

  private:
    AttributeListModel *_attributes;
};