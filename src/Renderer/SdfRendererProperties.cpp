#include "SdfRendererProperties.hpp"
#include "UI/Attribute/FloatAttribute.hpp"
#include "UI/Attribute/IntAttribute.hpp"

SdfRendererProperties::SdfRendererProperties(QObject *parent)
    : QObject(parent), _attributes(new AttributeListModel(this))
{
    // Connection for updates.
    auto res = connect(this, SIGNAL(valueChanged()), parent, SLOT(update()));
    if (!static_cast<bool>(res))
    {
        throw "The connection between signal and slot cannot be done.";
    }

    // Get the default attribute values.
    SdfRenderer_Params initialParams;

    // Initialize each attribute.
    _attributes->append(new FloatAttribute(
        "absorptionCoefficient", "Absorption Coefficient",
        initialParams.absorptionCoefficient, 0.f, 1.f, 0.01f, _attributes));
    _attributes->append(new FloatAttribute(
        "absorptionCutoff", "Absorption Cut-Off",
        initialParams.absorptionCutoff, 0.f, 1.f, 0.01f, _attributes));
    _attributes->append(new FloatAttribute(
        "marchMultiplier", "March Multiplier", initialParams.marchMultiplier,
        0.f, 5.f, 0.1f, _attributes));
    _attributes->append(new IntAttribute(
        "maxVolumeMarchSteps", "Max Volume March Steps",
        initialParams.maxVolumeMarchSteps, 0, 100, 1, _attributes));
    _attributes->append(new IntAttribute(
        "maxVolumeLightMarchSteps", "Max Volume Light March Steps",
        initialParams.maxVolumeLightMarchSteps, 0, 10, 1, _attributes));
    _attributes->append(new IntAttribute(
        "maxSdfSphereSteps", "Max SdF Sphere Steps",
        initialParams.maxSdfSphereSteps, 0, 50, 1, _attributes));
    _attributes->append(new IntAttribute(
        "maxOpaqueShadowMarchSteps", "Max Opaque Shadow March Steps",
        initialParams.maxOpaqueShadowMarchSteps, 0, 100, 1, _attributes));
}

const SdfRenderer_Params
SdfRendererProperties::sdfRenderer_Params() const
{
    SdfRenderer_Params params;

    params.absorptionCoefficient
        = _attributes->find("absorptionCoefficient")->getValue().toFloat();
    params.absorptionCutoff
        = _attributes->find("absorptionCutoff")->getValue().toFloat();
    params.marchMultiplier
        = _attributes->find("marchMultiplier")->getValue().toFloat();
    params.maxVolumeMarchSteps
        = _attributes->find("maxVolumeMarchSteps")->getValue().toInt();
    params.maxVolumeLightMarchSteps
        = _attributes->find("maxVolumeLightMarchSteps")->getValue().toInt();
    params.maxSdfSphereSteps
        = _attributes->find("maxSdfSphereSteps")->getValue().toInt();
    params.maxOpaqueShadowMarchSteps
        = _attributes->find("maxOpaqueShadowMarchSteps")->getValue().toInt();

    return params;
}
