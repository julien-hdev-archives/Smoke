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

    _attributes->append(new IntAttribute("nbParticules", "Number of particules",
                                           initialParams.nbParticules, 0, 50000,
                                           1., _attributes));

    _attributes->append(new IntAttribute("haveToReset", "Reset particules",
                                           initialParams.haveToReset, 0, 1,
                                           1, _attributes));
    
    _attributes->append(new FloatAttribute("radius", "Radius",
                                           initialParams.radius, 0.1f, 3.0f,
                                           0.1f, _attributes));

    _attributes->append(new FloatAttribute("viscosity", "Viscosity",
                                           initialParams.viscosity, 0.1f, 0.5f,
                                           0.01f, _attributes));

    _attributes->append(new FloatAttribute("pA", "pA",
                                           initialParams.pA, 0.1f, 3.0f,
                                           0.1f, _attributes));
}

const Simulator_Params
SimulatorProperties::simulator_Params() const
{
    Simulator_Params params;

    params.nbParticules = _attributes->find("nbParticules")->getValue().toInt();
    params.haveToReset = _attributes->find("haveToReset")->getValue().toInt();
    params.radius = _attributes->find("radius")->getValue().toFloat();
    params.viscosity = _attributes->find("viscosity")->getValue().toFloat();
    params.pA = _attributes->find("pA")->getValue().toFloat();

    return params;
}
