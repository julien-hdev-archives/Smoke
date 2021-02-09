#pragma once
#include "Scene/Components.hpp"
#include "Scene/Mesh.hpp"
#include "Scene/Scene.hpp"
#include <entt.hpp>

class Triangle2D : public Mesh
{
  public:
    Triangle2D () = default;
    Triangle2D (entt::entity handle, Scene *scene,
                const std::string &name = std::string (),
                const TransformComp &transform = TransformComp (),
                const SpriteColorComp &color = SpriteColorComp ());
};