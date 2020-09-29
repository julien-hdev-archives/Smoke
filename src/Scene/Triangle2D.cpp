#include "Triangle2D.hpp"

Triangle2D::Triangle2D(entt::entity handle,
	Scene* scene,
	const std::string& name,
	const TransformComp& transform,
	const SpriteColorComp& color
)
	: Mesh(handle, scene, name, transform, MeshComp({
		glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),
		glm::vec4( 0.5f, -0.5f, 0.0f, 1.0f),
		glm::vec4(-0.0f,  0.5f, 0.0f, 1.0f)
	}))
{
	AddComponent<SpriteColorComp>(color);
}