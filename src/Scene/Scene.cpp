#include "Scene.hpp"

//#include <entt.hpp>

void Scene::initialize() {
    _triangle.initialize();
}

void Scene::render() {
    _triangle.render();
}