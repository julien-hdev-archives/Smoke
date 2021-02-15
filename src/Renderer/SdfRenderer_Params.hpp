#pragma once

#include "Core/FloatAttribute.hpp"
#include "Utility/ObjectListModel.hpp"

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
    Q_PROPERTY(ObjectListModel *attributes MEMBER _attributes NOTIFY
                   attributesChanged)
  public:
    SdfRendererProperties(QObject *parent = nullptr) : QObject(parent)
    {
        SdfRenderer_Params initialParams;
        QObjectList objList;

        _attributes = new ObjectListModel(this);

        // Initialize each attribute.
        // Only one for now, for the test.
        auto absorptionCoefficient = new FloatAttribute(
            "absorptionCoefficient", "Absorption Coefficient",
            initialParams.absorptionCoefficient, 0.f, 1.f, 0.01f, _attributes);
        objList.append(absorptionCoefficient);

        // Populate the ListModel.
        _attributes->populate(objList);
    }
    ~SdfRendererProperties() = default;

  public:
    Q_SIGNAL void attributesChanged();

  private:
    ObjectListModel *_attributes;
};