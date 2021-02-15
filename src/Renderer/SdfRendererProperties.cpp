#include "SdfRendererProperties.hpp"

SdfRendererProperties::SdfRendererProperties(QObject *parent)
    : QObject(parent), _attributes(new AttributeListModel(this))
{
    // Connection for updates.
    auto res = connect(this, SIGNAL(valueChanged()), parent, SLOT(update()));
    if (!static_cast<bool>(res))
    {
        throw new std::exception(
            "The connection between signal and slot cannot be done.");
    }

    // Get the default attribute values.
    SdfRenderer_Params initialParams;

    // Initialize each attribute.
    // Only one for now, for the test.
    _attributes->append(new FloatAttribute(
        "absorptionCoefficient", "Absorption Coefficient",
        initialParams.absorptionCoefficient, 0.f, 1.f, 0.01f, _attributes));
}

const SdfRenderer_Params
SdfRendererProperties::sdfRenderer_Params() const
{
    SdfRenderer_Params params;

    params.absorptionCoefficient
        = _attributes->find("absorptionCoefficient")->getValue().toFloat();

    return params;
}