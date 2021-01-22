#include <glm/glm.hpp>
#include <array>
#include <vector>
#include <random>
#include <iostream>
#include <fstream>
#include <sstream>

#include <chrono>
#include <time.h>


    const int SIZE = 5;
    const int CLUSTER_SIZE = 10;
    const int TOTAL_SIZE = SIZE*CLUSTER_SIZE;

struct Particle
{
    glm::vec3 pos;
    glm::vec3 vel;
};


void
export_image(int it, const std::vector<Particle> & particles, float h_img)
{
    std::ostringstream name;
    name << "images/it" << it << ".pgm";
    int dim = (int) (((float) TOTAL_SIZE) /h_img);
    int image[dim][dim];
    for (auto y = 0; y < dim; ++y)
    {
        for (auto x = 0; x < dim; ++x)
        {
            image[x][y] = 0;
        }
    }

    int max_val = 10;
    for (auto & part: particles)
    {
        int x = (int) (part.pos[0]/h_img);
        int y = (int) (part.pos[1]/h_img);
        if (!(x < 0 || y < 0 || x >= dim || y >= dim))
        {
            if (image[x][y] < max_val)
                image[x][y]++;
        }            
    }

    std::ofstream img(name.str());

    img << "P2" << std::endl;
    img << dim << " " << dim << std::endl;
    img << max_val << std::endl;
    for (auto y = 0; y < dim; ++y)
    {
        for (auto x = 0; x < dim; ++x)
        {
            img << image[x][y] << " ";
        }

        img << std::endl;

    }
}


void
intermediate_speed(std::array<std::array<glm::vec3, TOTAL_SIZE>, TOTAL_SIZE> & grid,
                   std::vector<Particle> & particles,
                   float h)
{
        for (int x = 0; x < TOTAL_SIZE; ++x)
        {
            for (int y = 0; y < TOTAL_SIZE; ++y)
            {
                grid[x][y] = glm::vec3(0);
            }
        }

        // I just sum speed because meh
        for (auto & part: particles)
        {
            int x = (int) part.pos[0]/h;
            int y = (int) part.pos[1]/h;
            if (!(x < 0 || y < 0 || x >= TOTAL_SIZE || y >= TOTAL_SIZE))
            {
                grid[x][y] += part.vel;
            }
        }

        for (int x = 0; x < TOTAL_SIZE; ++x)
        {
            for (int y = 0; y < TOTAL_SIZE; ++y)
            {
                auto & u = grid[x][y];

                int x_past = (int) ((((float) x)*h - u[0])/h);
                int y_past = (int) ((((float) y)*h - u[1])/h);

                if (x_past < 0 || y_past < 0 || x_past >= TOTAL_SIZE || y_past >= TOTAL_SIZE)
                {
//                    std::cout << "OOB: "<< x_past << " " << y_past <<std::endl;
                    x_past = 0;
                    y_past = 0;
                }

                // maybe interpolate here
                auto u_past = grid[x_past][y_past];
                auto u_inter = u_past - glm::vec3(0); // no ext force for now
                grid[x][y] = u_inter;
            }
        }
    
}


void
vortricity(std::array<std::array<glm::vec3, TOTAL_SIZE>, TOTAL_SIZE> & grid)
{    
            // vortricity
        for (int x = 1; x < TOTAL_SIZE - 1; ++x)
        {
            for (int y = 1; y < TOTAL_SIZE - 1; ++y)
            {
                // du_y/dx
                auto duy = (grid[x+1][y][1] - grid[x-1][y][1])/2.0;
                // du_x/dy
                auto dux = (grid[x][y+1][0] - grid[x][y-1][0])/2.0;

                grid[x][y] = glm::vec3(0, 0, duy - dux);
            }
        }
        // because why not
        for (int x = 0; x < TOTAL_SIZE; ++x)
        {
            grid[x][0] = glm::vec3(0);
            grid[0][x] = glm::vec3(0);
            grid[x][TOTAL_SIZE-1] = glm::vec3(0);
            grid[TOTAL_SIZE-1][x] = glm::vec3(0);
        }
}


void
naive_approach(std::array<std::array<glm::vec3, TOTAL_SIZE>, TOTAL_SIZE> & grid,
               std::vector<Particle> & particles,
               float h,
               int d, float B)
{
       for (auto & part: particles)
        {
            int x = (int) part.pos[0]/h;
            int y = (int) part.pos[1]/h;

            glm::vec3 sum = glm::vec3(0);
            
            for (int x = 0; x < TOTAL_SIZE; ++x)
            {
                for (int y = 0; y < TOTAL_SIZE; ++y)
                {
                    glm::vec3 pos_i = glm::vec3(((float)x*h), ((float)y)*h, 0.);
                    auto dpos = part.pos - pos_i;
                    sum += glm::cross(grid[x][y], dpos) / (float) std::pow(std::max(glm::dot(dpos, dpos), h), d);
                }
            }

            auto u_new = B*sum;
            
            part.vel = u_new;

            part.pos += part.vel;
            if (part.pos[0] < 0.)
            {
                part.pos[0] = 0.;
                part.vel[0] = -part.vel[0];
            }
            if (part.pos[0] > TOTAL_SIZE*h-0.01)
            {
                part.pos[0] = TOTAL_SIZE*h-0.01;
                part.vel[0] = -part.vel[0];
            }
            if (part.pos[1] < 0.)
            {
                part.pos[1] = 0.;
                part.vel[1] = -part.vel[0];
            }
            if (part.pos[1] > TOTAL_SIZE*h-0.01)
            {
                part.pos[1] = TOTAL_SIZE*h-0.01;
                part.vel[1] = -part.vel[0];
            }
            
            
        }
}



struct Timer
{
    std::vector<std::chrono::time_point<
                    std::chrono::high_resolution_clock,
                    std::chrono::nanoseconds
                    >> dates;
    Timer()
    {
        log();
    }
    void log()
    {
        std::chrono::time_point<
                    std::chrono::high_resolution_clock,
                    std::chrono::nanoseconds
                    >
            t = std::chrono::high_resolution_clock::now();
        dates.push_back(t);
    }

    void
    print()
    {
        for (int i = 1; i < dates.size(); ++i)
        {
            float delta = std::chrono::duration_cast<std::chrono::milliseconds>(dates[i] - dates[i-1]).count();
            std::cout << "Measure n°" << i << " :  " << delta << " ms"<< std::endl;
        }
    }
    
};


int
main(void)
{
    // Seed with a real random value, if available
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_real_distribution<float> uniform_dist(-1.0, 1.0);

    
    std::vector<Particle> particles;


    const int d = 2;
    const float A = 2;
    const float h = 1.0;
    const float h_img = 0.25;
    const float dt = 0.1;

    const float B = std::pow(h, d)/A;
    int x_source = TOTAL_SIZE/2;
    int y_source = TOTAL_SIZE/2;
    
    int n = 10000;

    for (auto i = 0; i < n; ++i)
    {
        float vx = uniform_dist(e1);
        float vy = uniform_dist(e1);
        float x = h*(float) x_source + uniform_dist(e1)*5.0;
        float y = h*(float) y_source + uniform_dist(e1)*5.0;

        particles.push_back({
                glm::vec3(x, y, 0),
                glm::vec3(vx, vy, 0),
            });       
    }

    std::array<std::array<glm::vec3, TOTAL_SIZE>, TOTAL_SIZE> grid;

    
    for (int it = 0; it < 1000; ++it)
    {
        std::cout << "Iteration n°" << it << std::endl;

        Timer timer;
        
        intermediate_speed(grid, particles, h);

        timer.log();
        
        vortricity(grid);

        timer.log();
        
        naive_approach(grid, particles, h, d, B);

        timer.log();
        
        export_image(it, particles, h_img);

        timer.log();

        timer.print();

        

    }
    
    
    
    
    
    return 0;
}
