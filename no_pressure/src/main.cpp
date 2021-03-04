#include <glm/glm.hpp>
#include <array>
#include <vector>
#include <random>
#include <iostream>
#include <fstream>
#include <sstream>

#include <chrono>
#include <cstdlib>
#include <math.h>

struct Particle
{
    glm::vec3 pos;
    glm::vec3 vel;
};


#include "hybrid_simulator.hh"


int
main(void)
{
    // Seed with a real random value, if available
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_real_distribution<float> uniform_dist(-1.0, 1.0);
    std::mt19937 e2{r()};
    

    auto sim = Simulator<128, 8, 256>(1.0f, 2.0f);

    for (auto i = 0; i < 1000; ++i)
    {
        float vx = uniform_dist(e1)*0.001;
        float vy = uniform_dist(e1)*0.001;
        float x = uniform_dist(e1)*0.2;
        float y = uniform_dist(e1)*0.2;
        Particle part =
            {
                glm::vec3(x, y, 0),
                glm::vec3(vx, vy, 0),
            };
        
        sim.insert_particle(part);
    }

    for (auto i = 0; i < 100; ++i)
    {
        std::cout << "Iteration: " << i << std::endl;
        sim.run(0.5f);
        sim.update_densities();

        std::ostringstream name;
        name << "images/it" << i << ".pgm";
        const std::string name_s = name.str();
        sim.save_image(name_s, 0, 10);

    }
    
    
    return 0;
}
