#include "Scene.hpp"

#include "Scene/Entity.hpp"
#include "Scene/Mesh.hpp"
#include "Scene/Triangle2D.hpp"

#include <glm/glm.hpp>
#include <QtGui/QOpenGLFunctions>

#include "Renderer/Renderer.hpp"

#include "Utility/File.hpp"

	Scene::Scene()
        : _sdfRenderer(MyFile::LoadToString("shaders/fullscreen2.fs.glsl"))
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

	void Scene::OnRender()
    {
        QOpenGLFunctions* functions = QOpenGLContext::currentContext()->functions();
        functions->glClearColor(0.f, 0.f, 0.f, 1.f);
        functions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        _sdfRenderer.render();
	}

