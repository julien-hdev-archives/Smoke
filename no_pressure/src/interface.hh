struct SimulatorInterface
{
    // returns the density at the given coordinates
    virtual float
    get_density(float x, float y) = 0;
};
    
