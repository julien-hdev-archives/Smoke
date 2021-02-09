#pragma once
#include "Scene/Components.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Scene.hpp"
#include <entt.hpp>

class Mesh : public Entity
{
  public:
    Mesh () = default;

    Mesh (entt::entity handle, Scene *scene,
          const std::string &name = std::string (),
          const TransformComp &transform = TransformComp (),
          const MeshComp &mesh = MeshComp ());
};