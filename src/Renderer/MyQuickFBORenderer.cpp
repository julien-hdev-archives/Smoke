#include "MyQuickFBORenderer.hpp"

#include "MyQuickFBO.hpp"
#include <QtGui/QOpenGLFramebufferObject>
#include <QtGui/QOpenGLFunctions>
#include <QtQuick/QQuickWindow>

#include <algorithm>

#include <random>


MyQuickFBORenderer::MyQuickFBORenderer(QQuickWindow &window) : _window(window)
{
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_real_distribution<float> uniform_dist(-1.0, 1.0);

    for (auto i = 0; i < 10000; ++i)
    {
        float vx = uniform_dist(e1) * 0.001;
        float vy = uniform_dist(e1) * 0.001;
        float x = uniform_dist(e1) * 0.2;
        float y = uniform_dist(e1) * 0.2;
        Particle part = { glm::vec3(x, y, 0), glm::vec3(vx, vy, 0), 0., 0. };

        simulator.insert_particle(part);
    }
}

void
MyQuickFBORenderer::resetSimulation()
{

    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_real_distribution<float> uniform_dist(-1.0, 1.0);

    simulator.clear_particle();

    for (auto i = 0; i < 10000; ++i) // simulator.nbparticules
    {
        float vx = uniform_dist(e1) * 0.001;
        float vy = uniform_dist(e1) * 0.001;
        float x = uniform_dist(e1) * 0.2;
        float y = uniform_dist(e1) * 0.2;
        Particle part = { glm::vec3(x, y, 0), glm::vec3(vx, vy, 0), 0., 0. };

        simulator.insert_particle(part);
    }
}

void
MyQuickFBORenderer::synchronize(QQuickFramebufferObject *item)
{
    MyQuickFBO *quickFBO = reinterpret_cast<MyQuickFBO *>(item);

    _sdfRendererParams = quickFBO->sdfRenderer_Params();
    quickFBO->camera().update();
    _cameraInfos = quickFBO->camera().getInfos();

    auto simulatorParams = quickFBO->simulator_Params();
    simulator.update_param(simulatorParams);

    // Reset Simulation
    if (simulatorParams.haveToReset >= 0.99) resetSimulation();
    quickFBO->setHaveToReset(0.f);
}

void
MyQuickFBORenderer::render()
{
    QOpenGLFunctions *functions
        = QOpenGLContext::currentContext()->functions();
    functions->glDisable(GL_DEPTH_TEST);

    //_sdfRenderer.render(_cameraInfos, _sdfRendererParams);
    //_sdfRenderer2D.render(_simulation.gridSize());


    simulator.run();
    simulator.update_densities();

    const auto w = DENSITY_GRID_SIZE;
    const auto h = DENSITY_GRID_SIZE;
    const auto n = w*h;
    
    std::vector<unsigned char> data;

    long long total = 0;
    
    for (auto it = simulator.density_begin();
         it != simulator.density_end();
         ++it)
    {
        int density = (*it)*10;

        auto r = (unsigned char) std::min(density, 255);
        auto g = (unsigned char) std::min(density, 255);
        auto b = (unsigned char) std::min(density, 255);
        auto a = (unsigned char) 255;

        data.push_back(r);
        data.push_back(g);
        data.push_back(b);
        data.push_back(a);

        total += density;
        
    }

    std::cout << "TOTAL MATTER: " << total << std::endl;
    _imageRenderer.render(w, h, data.data());
    _simulation.update();
    update();
    _window.resetOpenGLState();
}

QOpenGLFramebufferObject *
MyQuickFBORenderer::createFramebufferObject(const QSize &size)
{
    QOpenGLFramebufferObjectFormat format;
    return new QOpenGLFramebufferObject(size, format);
}
