#include "Entity.hpp"

Entity::Entity (entt::entity handle, Scene *scene, const std::string &name,
                const TransformComp &transform)
    : m_EntityHandle (handle), m_Scene (scene)
{
    AddComponent<TransformComp> ();
    AddComponent<TagComp> ((name.empty ()) ? "Entity" : name);
}