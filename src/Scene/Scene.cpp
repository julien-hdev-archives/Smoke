#include "Scene.hpp"

#include "Scene/Entity.hpp"
#include "Scene/Mesh.hpp"
#include "Scene/Triangle2D.hpp"

#include <glm/glm.hpp>
#include <QtGui/QOpenGLFunctions>

#include "Renderer/Renderer.hpp"

#include "Utility/File.hpp"

	Scene::Scene()
	{
		_sdfRenderer.setScene(MyFile::LoadToString("shaders/simulation/SDF_Test1.fs.glsl"));
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

	void Scene::onRender()
    {
        QOpenGLFunctions* functions = QOpenGLContext::currentContext()->functions();
        functions->glClearColor(0.f, 0.f, 0.f, 1.f);
        functions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        _sdfRenderer.render(_camera);
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
