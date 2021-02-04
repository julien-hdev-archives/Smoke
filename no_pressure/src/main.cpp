#include <glm/glm.hpp>
#include <array>
#include <vector>
#include <random>
#include <iostream>
#include <fstream>
#include <sstream>

#include <chrono>
#include <cstdlib>

const int SIZE = 8;
const int CLUSTER_SIZE = 16;
const int TOTAL_SIZE = SIZE*CLUSTER_SIZE;
const int n = 10000;
const float bronian_strength = 0.5;
const int d = 2;
const float A = 2.;
const float h = 1.;
const float h_img = 0.25;
const float dt = 0.0001;

struct Particle
{
    glm::vec3 pos;
    glm::vec3 vel;
};


void
export_image(int it, const std::vector<Particle> & particles, float h, float h_img)
{
    std::cout << "MARCO\n";
    std::ostringstream name;
    name << "images/it" << it << ".pgm";
    int dim = (int) ((h*(float) TOTAL_SIZE) / h_img);
    int image[dim][dim];
    std::cout << "MARCO000\n";
    for (auto y = 0; y < dim; ++y)
    {
        for (auto x = 0; x < dim; ++x)
        {
            image[x][y] = 0;
        }
    }
    std::cout << "MARCO\n";
    int max_val = 255;
    int min_val = 40;
    for (auto & part: particles)
    {
        int x0 = (int) (part.pos[0]/h_img);
        int y0 = (int) (part.pos[1]/h_img);

        /*
        int x = x0;
        int y = y0;
        if (!(x < 0 || y < 0 || x >= dim || y >= dim))
        {
            image[x][y]+= 1;
            if (image[x][y] > max_val)
                image[x][y] = max_val;
        }
        */
        
        
        int r = 0;
        for (int dx = -r; dx <= r; ++dx)
        {
            for (int dy = -r; dy <= r; ++dy)
            {
                int x = x0+dx;
                int y = y0+dy;
                
                int v = 1+glm::max<int>(0, 2*r - (std::abs(dx)+std::abs(dy)));
                if (!(x < 0 || y < 0 || x >= dim || y >= dim))
                {
                    image[x][y]+= v;
                    if (image[x][y] > max_val)
                        image[x][y] = max_val;
                }
            }
        }
        
    }
    std::cout << "MARCO\n";

    std::ofstream img(name.str());

    img << "P2" << std::endl;
    img << dim << " " << dim << std::endl;
    img << max_val << std::endl;
    for (auto y = 0; y < dim; ++y)
    {
        for (auto x = 0; x < dim; ++x)
        {
            auto v = image[x][y];
            if (v > 0 && v < min_val) v = min_val;
            img << v << " ";
        }

        img << std::endl;

    }
}


void
intermediate_speed(std::array<std::array<glm::vec3, TOTAL_SIZE>, TOTAL_SIZE> & grid,
                   std::vector<Particle> & particles,
                   float h,
                   float dt)
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
        {            int x = (int) part.pos[0]/h;
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

                int x_past = (int) ((((float) x)*h - u[0]*dt)/h);
                int y_past = (int) ((((float) y)*h - u[1]*dt)/h);

                if (x_past < 0 || y_past < 0 || x_past >= TOTAL_SIZE || y_past >= TOTAL_SIZE)
                {
                    x_past = 0;
                    y_past = 0;
                }

                // maybe interpolate here
                auto u_past = grid[x_past][y_past];

                float mid = (h*(float) TOTAL_SIZE)/2.0;
                float rx = h*(float) x;
                float ry = h*(float) y;

                float fint = glm::max<float>(0., - (rx - mid/2.0)*(rx - mid/2.0*3.0));
                
                auto f = glm::vec3(0, fint, 0);
                f = glm::vec3(0.0, 10.0, 0.0);
                auto u_inter = u_past + f*dt; // no ext force for now
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

                grid[x][y] = glm::vec3(0, 0, (duy - dux)*2.);
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


void // prints the angles
print_vortricity(std::array<std::array<glm::vec3, TOTAL_SIZE>, TOTAL_SIZE> & grid)
{
    for (int i = 0; i < TOTAL_SIZE; ++i)
    {
        for (int j = 0; j < TOTAL_SIZE; ++j)
        {
            std::cout << " " << grid[i][j][2];
        }
        std::cout << std::endl;
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

void
smart_approach(std::array<std::array<glm::vec3, TOTAL_SIZE>, TOTAL_SIZE> & grid,
               std::array<std::array<glm::vec3, TOTAL_SIZE>, TOTAL_SIZE> & speed,
               std::vector<Particle> & particles,
               float h,
               int d,
               float B,
               std::array<std::array<std::array<glm::vec3, 3>, SIZE>, SIZE> & precalc,
               float dt)
{



    for (int clust_i = 0; clust_i < SIZE; ++clust_i)
    {
        for (int clust_j = 0; clust_j < SIZE; ++clust_j)
        {
            auto xref = glm::vec3(h*(((float) SIZE)/2.0 + ((float) clust_i)),
                                  h*(((float) SIZE)/2.0 + ((float) clust_j)),
                                  0.0f);
            
            auto sum1 = glm::vec3(0);
            auto sum2 = glm::vec3(0);
            for (int local_i = 0; local_i < CLUSTER_SIZE; ++local_i)
            {
                for (int local_j = 0; local_j < CLUSTER_SIZE; ++local_j)
                {
                    int i = clust_i*CLUSTER_SIZE + local_i;
                    int j = clust_j*CLUSTER_SIZE + local_j;

                    auto x = glm::vec3(h*((float) i),h*((float) j), 0.0f);
                    auto & s = grid[i][j];

                    sum1 += s;
                    sum2 += glm::cross(s, x);
                }
            }

            precalc[clust_i][clust_j][0] = sum1;
            precalc[clust_i][clust_j][1] = sum2;
            precalc[clust_i][clust_j][2] = xref;
            
        }
    }



    for (int x = 0; x < TOTAL_SIZE; ++x)
    {
        for (int y = 0; y < TOTAL_SIZE; ++y)
        {
            
            int clust_x = x/CLUSTER_SIZE;
            int clust_y = y/CLUSTER_SIZE;
            glm::vec3 pos = glm::vec3(h*(float)x, h*(float)y, 0.0f);
            glm::vec3 sum = glm::vec3(0);
            int radius = 1;
            int min_x = glm::max<int>(0, clust_x-radius);
            int max_x = glm::min<int>(SIZE, clust_x+radius+1);
            int min_y = glm::max<int>(0, clust_y-radius);
            int max_y = glm::min<int>(SIZE, clust_y+radius+1);

            //  std::cout << "("<<x<<" "<<y<< ")" << " ("<<clust_x<<" "<<clust_y<< ")" << "  WINDOW: " << min_x << " "<< min_y << " "<< max_x << " "<< max_y << std::endl;
/*
            for (int x = min_x*CLUSTER_SIZE; x < max_x*CLUSTER_SIZE; ++x)
            {
                for (int y = min_y*CLUSTER_SIZE; y < max_y*CLUSTER_SIZE; ++y)
                {
                    glm::vec3 pos_i = glm::vec3(((float)x*h), ((float)y)*h, 0.);
                    auto dpos = pos - pos_i;
                    sum += glm::cross(grid[x][y], dpos) / (float) std::pow(std::max(glm::dot(dpos, dpos), h), d);
                }
            }
*/
            // approximation for the distant clusters
            for (int clust_i = 0; clust_i < SIZE; ++clust_i)
            {
                if (abs(clust_i - clust_x) <= 1) continue;
                for (int clust_j = 0; clust_j < SIZE; ++clust_j)
                {
                    if (abs(clust_j - clust_y) <= 1) continue;

                    auto & s1 = precalc[clust_i][clust_j][0];
                    auto & s2 = precalc[clust_i][clust_j][1];
                    auto & xref = precalc[clust_i][clust_j][2];
                    auto dpos = pos - xref;
                    sum += (glm::cross(s1, pos) - s2) / (float) std::pow(std::max(glm::dot(dpos, dpos), h), d);
                }
            }
      
            auto u_new = B*sum;

            speed[x][y] = u_new;
        
            /*
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
            */
            
        }
    }
    
    for (auto & part: particles)
    {
        int x0 = (int) (part.pos[0]/h);
        int y0 = (int) (part.pos[1]/h);

            
        int clust_x = x0/CLUSTER_SIZE;
        int clust_y = y0/CLUSTER_SIZE;
        glm::vec3 & pos = part.pos;
        glm::vec3 sum = glm::vec3(0);
        int radius = 1;
        int min_x = glm::max<int>(0, clust_x-radius);
        int max_x = glm::min<int>(SIZE, clust_x+radius+1);
        int min_y = glm::max<int>(0, clust_y-radius);
        int max_y = glm::min<int>(SIZE, clust_y+radius+1);


        for (int x = min_x*CLUSTER_SIZE; x < max_x*CLUSTER_SIZE; ++x)
        {
            for (int y = min_y*CLUSTER_SIZE; y < max_y*CLUSTER_SIZE; ++y)
            {
                glm::vec3 pos_i = glm::vec3(((float)x*h), ((float)y)*h, 0.);
                auto dpos = pos - pos_i;
                sum += glm::cross(grid[x][y], dpos) / (float) std::pow(std::max(glm::dot(dpos, dpos), h), d);
            }
        }



        if (x0 < 0 || x0 >= TOTAL_SIZE || y0 < 0 || y0 >= TOTAL_SIZE)
        {
            float mid = h/2.0*(float) TOTAL_SIZE;
            part.pos = glm::vec3(mid, mid, 0.0f);
        }
        else
        {
            /*
            glm::vec3 sum_speed = glm::vec3(0);
            for (int dx = -1; dx <= 1; ++dx)
            {
                for (int dy = -1; dy <= 1; ++dy)
                {
                    int x = x0+dx;
                    int y = y0+dy;
                    float rx = h*(float) x;
                    float ry = h*(float) y;
                    float d = glm::distance(glm::vec3(rx, ry, 0.0f), part.pos);
                    if (!(x < 0 || x >= TOTAL_SIZE || y < 0 || y >= TOTAL_SIZE))
                    {
                        sum_speed += speed[x][y]/(d+1.0f);
                    }
                }
            }
            */
            sum += speed[x0][y0];
            part.vel = B*sum;
            part.pos += part.vel*dt;
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

void
broonian(std::vector<Particle> & particles,
         std::mt19937 & rng)
{
    std::normal_distribution<float> gaussian;
    for (auto & part: particles)
    {
        float dx = gaussian(rng)*bronian_strength;
        float dy = gaussian(rng)*bronian_strength;
        part.pos[0] += dx;
        part.pos[1] += dy;
    }
}


int
main(void)
{
    // Seed with a real random value, if available
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_real_distribution<float> uniform_dist(-1.0, 1.0);
    std::mt19937 e2{r()};
    
    std::vector<Particle> particles;
    std::array<std::array<std::array<glm::vec3, 3>, SIZE>, SIZE> precalc;

    std::array<std::array<glm::vec3, TOTAL_SIZE>, TOTAL_SIZE> grid;
    std::array<std::array<glm::vec3, TOTAL_SIZE>, TOTAL_SIZE> speed;

    
    const float B = std::pow(h, d)/A;
    int x_source = TOTAL_SIZE/2;
    int y_source = TOTAL_SIZE/2;
    

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

    
    for (int it = 0; it < 1000; ++it)
    {
        std::cout << "Iteration n°" << it << std::endl;

        Timer timer;
        
        intermediate_speed(grid, particles, h, dt);

        timer.log();
        
        vortricity(grid);

        //print_vortricity(grid);
        timer.log();


        if (true)
            smart_approach(grid, speed, particles, h, d, B, precalc, dt);
        else
            naive_approach(grid, particles, h, d, B);
        
        timer.log();

        broonian(particles,  e2);
        
        timer.log();
        export_image(it, particles, h, h_img);

        timer.log();

        timer.print();

        

    }
    
    
    
    
    
    return 0;
}
