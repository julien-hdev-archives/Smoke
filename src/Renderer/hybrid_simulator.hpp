
#include <array>
#include <fstream>

#include "Renderer/SimulatorProperties.hpp"

#include "interface.hpp"
#include "timer.hpp"

#include <glm/glm.hpp>
#include <random>

struct Particle
{
    glm::vec3 pos;
    glm::vec3 vel;
    float mass;
    float heat;
};

// CLUST_SIZE must divide GRID_SIZE
template<int GRID_SIZE, int CLUST_SIZE, int DENSITY_GRID_SIZE>
struct Simulator2D: public SimulatorInterface
{
    
    using Grid = std::array<std::array<glm::vec3, GRID_SIZE>, GRID_SIZE>;
    using ClustGrid = std::array<std::array<std::array<glm::vec3, 3>, GRID_SIZE/CLUST_SIZE>, GRID_SIZE/CLUST_SIZE>;
    using DensityGrid = std::array<std::array<int, DENSITY_GRID_SIZE>, DENSITY_GRID_SIZE>;

    const int CLUST_GRID_SIZE = GRID_SIZE/CLUST_SIZE;

    // I store the points in a quadtree rather than in a vector, for performance purpose
    std::vector<Particle> particles;
    Grid vel_grid = {};
    Grid inter_vel_grid = {};
    Grid vortricity_grid = {};

    ClustGrid cluster_grid = {};

    DensityGrid density = {};
    
    float radius = 1.0f;
    float res = 2.0f/((float) GRID_SIZE);
    float density_res = 2.0f/((float) DENSITY_GRID_SIZE);
    float clust_res = 2.0f/((float) CLUST_GRID_SIZE);
    int dim = 2;
    float A = 2.0f;
    float B;

    float viscosity = 1.0f;

    float dt = 1.0f;
    
    float
    get_density(float x, float y)
    {
        const auto i = space_to_density_grid(x);
        const auto j = space_to_density_grid(y);

        return is_inside_density(i, j)? density[i][j] : 0.0f;
   }
    
    Simulator2D() = default;

    /*
      rad: radius of the simulation (the size in the particle space)
      pA: some constant that should be 2.0
      visc: how fast particles loose speed (must be within [0, 1])
     */
    Simulator2D(float rad, float pA = 2.0f, float visc = 1.0f)
        : radius(rad), res(2.0f * rad / ((float)GRID_SIZE)),
          density_res(2.0f * rad / ((float)DENSITY_GRID_SIZE)),
          clust_res(2.0f * rad / ((float)CLUST_GRID_SIZE)), A(pA),
          B(std::pow(res, dim) / A), viscosity(visc)
    {}

    void
    update_param(Simulator_Params params)
    {
        float rad = 1.0f;
        float pA = 2.0f;

        radius = rad;
        res = 2.0f * rad / ((float)GRID_SIZE);
        density_res = 2.0f * rad / ((float)DENSITY_GRID_SIZE);
        clust_res = 2.0f * rad / ((float)CLUST_GRID_SIZE);
        A = pA;
        B = std::pow(res, dim) / A;
        viscosity = params.viscosity;
    }

    void
    clear_particle()
    {
        particles.clear();
    }

    void
    set_dt(float pdt)
    {
        dt = pdt;
    }
    
    // inserters
    void
    insert_particle(const Particle & part)
    {
        particles.push_back(part);
    }

    void
    insert_particle(const float x,
                    const float y,
                    const float z)
    {
        insert_particle(Particle{glm::vec3(x, y, z), glm::vec3(0.0f)});
    }

    void
    insert_particle(const float x,
                    const float y) const
    {
        insert_particle(x, y, 0.0f);
    }

    
    float
    grid_to_space(const int i) const
    {
        // i+1 = 1 => x = -radius
        // i+1 = GRID_SIZE => x = radius
        // (i+1)*a - radius= x
        // a = (x+radius)/(i+1)
        // a = 2*radius/GRID_SIZE
        // x = (i+1)*a - radius
        return ((float) (i+1)) * res - radius;
    }
    glm::vec3
    grid_to_space(const int i, const int j) const
    {
        return glm::vec3(grid_to_space(i), grid_to_space(j), 0.0f);
    }
    
    int
    space_to_grid(const float x) const
    {
        // i+1 = (x + radius)/res
        return (x + radius) / res - 1;
    }

    int
    space_to_density_grid(const float x) const
    {
        // i+1 = (x + radius)/res
        return (x + radius) / density_res - 1;
    }

    bool
    is_inside(const int i) const
    {
        return i >= 0 && i < GRID_SIZE;
    }    

    bool
    is_inside_density(const int i) const
    {
        return i >= 0 && i < DENSITY_GRID_SIZE;
    }    

    bool
    is_inside_density(const int i, const int j) const
    {
        return is_inside_density(i) && is_inside_density(j);
    }    

    bool
    is_inside(const int i, const int j) const
    {
        return is_inside(i) && is_inside(j);
    }

    bool
    is_inside(const glm::vec3 & pos) const
    {
        return is_inside(space_to_grid(pos[0]),
                         space_to_grid(pos[1]));
    }

    bool
    is_inside_density(const glm::vec3 & pos) const
    {
        return is_inside_density(space_to_density_grid(pos[0]),
                                 space_to_density_grid(pos[1]));
    }

    
    float
    cluster_to_space(const int c) const
    {
        return ((float) (c+1)) * clust_res - radius;
    }


    int
    grid_to_cluster(const int i) const
    {
        return i / CLUST_SIZE;
    }

    glm::vec3 &
    get_cluster(const int i, const int j)
    {
        return cluster_grid[grid_to_cluster(i)][grid_to_cluster(j)];
    }
        
    void
    run()
    {
        Timer clock;
        collect_speeds();
        clock.log();
        intermediate_speed(dt);
        clock.log();
        vortricity(dt);

        clock.log();
 
        // algorithm (takes the totality of the computational time
        //naive_algorithm(dt);
        smart_algo(dt, 1);

        clock.log();

        brownian(res/100.0f);
        

        reflect_exterior_particles();        
        //     destroy_exterior_particles();
        clock.log();

        std::cout << "RUN CLOCK: \n";
        clock.print();
        
    }

    void
    collect_speeds()
    {
        // grid reset
        vel_grid = {};

        // sum particles speed in the corresponding cells
        for (const auto & part: particles)
        {
            const int i = space_to_grid(part.pos[0]);
            const int j = space_to_grid(part.pos[1]);
            if (is_inside(i, j))
                vel_grid[i][j] += part.vel;
        }
    }

    glm::vec3
    get_grid_element(const int i, const int j) const
    {
        return is_inside(i, j)? vel_grid[i][j] : glm::vec3(0.0f);
    }
    
    void
    intermediate_speed(float dt)
    {

        // intermediate speed calculation
        // it is the sum of the speed of the cell in the opposite direction
        // of the velocity and the speed that comes from the external forces
        for (int i = 0; i < GRID_SIZE; ++i)
        {
            for (int j = 0; j < GRID_SIZE; ++j)
            {
                const auto vx = vel_grid[i][j][0];
                const auto vy = vel_grid[i][j][1];

                const auto x = grid_to_space(i);
                const auto y = grid_to_space(j);
                
                const auto x_past = x - vx;
                const auto y_past = y - vy;

                const auto i_past = space_to_grid(x_past);
                const auto j_past = space_to_grid(y_past);

                // we consider the exterior to be empty
                const auto & u_past = get_grid_element(i_past, j_past);

                // eventual forces to take in account here or in another function

                auto u_inter = u_past; // + u_forces
                
                inter_vel_grid[i][j] = u_inter;
            }
        }
    }

    // computes the vorticity  of each cell (the parts that makes the smoke spin)
    void
    vortricity(float dt)
    {
        vortricity_grid = {};
        
        for (int i = 0; i < GRID_SIZE; ++i)
        {
            for (int j = 0; j < GRID_SIZE; ++j)
            {
                const auto & right = get_grid_element(i+1, j);
                const auto & left = get_grid_element(i-1, j);
                const auto & down = get_grid_element(i, j+1);
                const auto & up = get_grid_element(i, j-1);

                // there is a 2.0 factor that is being simplified out
                
                // x derivation of the y coordinate
                const auto duy = left[1] - right[1];
                // y derivation of the x coordinate
                const auto dux = down[0] - up[0];

                const auto vort = duy - dux;
                
                vortricity_grid[i][j] = glm::vec3(0, 0, vort);
            }
        }
    }

    // a naive implementation for comparison purpose
    void
    naive_algorithm(float dt)
    {
        for (auto & part: particles)
        {
            auto sum = glm::vec3(0.0f);

            for (int i = 0; i < GRID_SIZE; ++i)
            {
                for (int j = 0; j < GRID_SIZE; ++j)
                {
                    auto pos_i = glm::vec3(grid_to_space(i),
                                           grid_to_space(j), 0.0f);
                    auto translation = part.pos - pos_i;
                    auto dist2 = std::max(res*res, glm::dot(translation, translation));
                    auto dist_d = std::pow(dist2, ((float) dim) /2.0f);
                    sum += glm::cross(inter_vel_grid[i][j], translation) / dist_d;
                }
            }

            const auto u_new = B*sum;

            part.vel += u_new*dt;
            part.pos += part.vel*dt;
        }
    }

    void
    reflect_exterior_particles()
    {
        for (int i = particles.size() - 1; i >= 0; --i)
        {
            auto & x = particles[i].pos[0];
            auto & y = particles[i].pos[1];
            auto & vx = particles[i].vel[0];
            auto & vy = particles[i].vel[1];

            if (x < -radius)
            {
                x = -radius;
                vx = -vx;
            }
            if (x > +radius)
            {
                x = +radius;
                vx = -vx;
            }
            if (y < -radius)
            {
                y = -radius;
                vy = -vy;
            }
            if (y > +radius)
            {
                y = +radius;
                vy = -vy;
            }
            /*
            if (!is_inside(particles[i].pos) || !is_inside_density(particles[i].pos))
            {
                particles[i] = particles.back();
                particles.pop_back();
            }
            */
        }
    }
    
    void
    destroy_exterior_particles()
    {
        for (int i = particles.size() - 1; i >= 0; --i)
        {
            if (!is_inside(particles[i].pos) || !is_inside_density(particles[i].pos))
            {
                particles[i] = particles.back();
                particles.pop_back();
            }
        }
    }

    // must be called AFTER cleanup (will probably segfault if not)
    void
    update_densities()
    {
        density = {};
        for (const auto & part: particles)
        {
            const auto i = space_to_density_grid(part.pos[0]);
            const auto j = space_to_density_grid(part.pos[1]);
            density[i][j]++;
        }
    }


    // precalculate things and stores it in the cluster grid
    void
    compute_clusters()
    {
        for (auto ci = 0; ci < CLUST_GRID_SIZE; ++ci)
        {
            for (auto cj = 0; cj < CLUST_GRID_SIZE; ++cj)
            {
                auto xref = glm::vec3(cluster_to_space(ci),
                                      cluster_to_space(cj), 0.0f);
                auto sum1 = glm::vec3(0.0f);
                auto sum2 = glm::vec3(0.0f);

                for (auto i = ci*CLUST_SIZE; i < (ci+1)*CLUST_SIZE; ++i)
                {
                    for (auto j = cj*CLUST_SIZE; j < (cj+1)*CLUST_SIZE; ++j)
                    {
                        auto x = grid_to_space(i, j);
                        auto s = get_grid_element(i, j);
                        sum1 += s;
                        sum2 += glm::cross(s, x);
                    }
                }

                cluster_grid[ci][cj][0] = sum1;
                cluster_grid[ci][cj][1] = sum2;
                cluster_grid[ci][cj][2] = xref;
                
            }
            
        }
    }

    // I approximate the contribution of the distant clusters so that I can run
    // the algorithm in O(nb_particles*(nb_clusters + cluster_size))
    // rather than in O(nb_particles*nb_cells)
    void
    smart_algo(float dt, int clust_radius)
    {
        for (auto & part: particles)
        {
            auto i0 = space_to_grid(part.pos[0]);
            auto j0 = space_to_grid(part.pos[1]);
            auto ci0 = grid_to_cluster(i0);
            auto cj0 = grid_to_cluster(j0);

            auto sum = glm::vec3(0.0f);
                
            for (auto ci = 0; ci < CLUST_GRID_SIZE; ++ci)
            {
                for (auto cj = 0; cj < CLUST_GRID_SIZE; ++cj)
                {
                    if (std::max(std::abs(ci0-ci), std::abs(cj0-cj)) <= clust_radius)
                    {
                        // non-approximated case
                        for (auto i = ci*CLUST_SIZE; i < (ci+1)*CLUST_SIZE; ++i)
                        {
                            for (auto j = cj*CLUST_SIZE; j < (cj+1)*CLUST_SIZE; ++j)
                            {
                                const auto pos_i = glm::vec3(grid_to_space(i),
                                                       grid_to_space(j), 0.0f);
                                const auto translation = part.pos - pos_i;
                                const auto dist2 = std::max(res*res, glm::dot(translation, translation));
                                const auto dist_d = std::pow(dist2, ((float) dim) /2.0f);
                                sum += glm::cross(inter_vel_grid[i][j], translation) / dist_d;

                            }
                        }
                    }
                    else
                    {
                        // approximated case
                        const auto & s1 = cluster_grid[ci][cj][0];
                        const auto & s2 = cluster_grid[ci][cj][1];
                        const auto & xref = cluster_grid[ci][cj][2];
                        const auto translation = part.pos - xref;
                        const auto dist2 = std::max(res*res, glm::dot(translation, translation));
                        const auto dist_d = std::pow(dist2, ((float) dim) / 2.0f);

                        sum += (glm::cross(s1, part.pos) - s2) / dist_d;

                    }
                }
            }
            
            auto u_new = B*sum;

            part.vel += u_new*dt;
            part.pos += part.vel*dt;
            part.vel *= viscosity;
        }
    }

    // adds brownian movement to the particles
    // neglictable cost, enhance significantly the result
    void
    brownian(float force)
    {
        std::random_device rd{};
        std::mt19937 gen{rd()};
        std::normal_distribution<> gaussian{0.0f, force};

        for (auto & part: particles)
        {
            part.vel[0] += gaussian(gen);
            part.vel[1] += gaussian(gen);
        }
        
    }

    const int *
    density_begin()
    {
        return &density[0][0];
    }
    
    const int *
    density_end()
    {
        return DENSITY_GRID_SIZE*DENSITY_GRID_SIZE + &density[0][0];
    }

    
    
    void
    save_image(const std::string & name,
               const int min_val,
               const int max_val)
    {
        std::ofstream img(name);
        img << "P2" << std::endl;
        img << DENSITY_GRID_SIZE << " " << DENSITY_GRID_SIZE << std::endl;
        img << max_val << std::endl;

        for (auto p = density_begin(); p != density_end(); ++p)
        {
            auto v = *p;
            if (v > 0 && v < min_val)
                v = min_val;
            v = std::min(max_val, v);
            img << v << " ";
        }
        img << std::endl;
        

    }
    
};
   
