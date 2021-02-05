#include "Scene.hpp"

#include "Scene/Entity.hpp"
#include "Scene/Mesh.hpp"
#include "Scene/Triangle2D.hpp"

#include <glm/glm.hpp>
#include <QtGui/QOpenGLFunctions>

#include "Renderer/Renderer.hpp"

	Scene::Scene()
	{
	}
 
	Entity Scene::CreateEntity(const std::string& name, const TransformComp& transform)
	{
		Entity entity = { m_Registry.create(), this, name, transform };
		return entity;
	}

    
	Triangle2D Scene::CreateTriangle2D(const std::string& name, const TransformComp& transform, glm::vec4 color)
    {
        
		Triangle2D triangle2D = { m_Registry.create(), this, name, transform, SpriteColorComp(color) };
		return triangle2D;
	}

	void Scene::onUpdate()
	{
		_camera.update();
	}

	void Scene::onMousePress() {
		_camera.onWheelDown();
	}

	void Scene::onMouseRelease() {
		_camera.onWheelUp();
	}

	void Scene::onWheelScroll(float delta) {
		_camera.onWheelScroll(delta);
	}
