#include "SimulatorProperties.hpp"

SimulatorProperties::SimulatorProperties(QObject *parent)
    : QObject(parent), _attributes(new AttributeListModel(this))
{
    // Connection for updates.
    auto res = connect(this, SIGNAL(valueChanged()), parent, SLOT(update()));
    if (!static_cast<bool>(res))
    {
        throw "The connection between signal and slot cannot be done.";
    }

    // Get the default attribute values.
    Simulator_Params initialParams;

    // Initialize each attribute.
    // Only one for now, for the test.
    _attributes->append(new FloatAttribute("haveToReset", "Have To Reset",
                                           initialParams.haveToReset, 0.f, 1.f,
                                           1.f, _attributes));
    _attributes->append(new FloatAttribute("viscosity", "Viscosity",
                                           initialParams.viscosity, 0.1f, 0.5f,
                                           0.01f, _attributes));
}

const Simulator_Params
SimulatorProperties::simulator_Params() const
{
    Simulator_Params params;

    params.haveToReset
        = _attributes->find("haveToReset")->getValue().toFloat();
    params.viscosity = _attributes->find("viscosity")->getValue().toFloat();

    return params;
}
