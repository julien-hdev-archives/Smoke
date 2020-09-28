#ifndef SCENE_HPP
#define SCENE_HPP
#pragma once

#include "Triangle.hpp"

class Scene {
    public:
        Scene() = default;
        ~Scene() = default;

        void initialize();
        void render();

    private:
        Triangle _triangle;
};

#endif