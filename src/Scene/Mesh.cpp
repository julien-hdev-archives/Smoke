#include "Mesh.hpp"

Mesh::Mesh(entt::entity handle, Scene *scene, const std::string &name,
           const TransformComp &transform, const MeshComp &mesh)
    : Entity(handle, scene, name, transform)
{
    AddComponent<MeshComp>(mesh);
}