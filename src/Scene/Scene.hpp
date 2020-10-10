
/*#pragma once

#include "Triangle.hpp"

class Scene {
    public:
        Scene() = default;
        ~Scene() = default;

        void initialize();
        void render();

    private:
        Triangle _triangle;
};*/


#pragma once
//#include "Triangle.hpp"
//#include "Mesh.hpp"
#include "Scene/Components.hpp"
//#include "Triang.hpp"
#include <entt.hpp>

#include <QVector>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLVertexArrayObject>

#include "Renderer/FullscreenShader.hpp"

	class Entity;
	class Mesh;
	class Triangle2D;

	class Scene
	{
	public:
		Scene();
		virtual ~Scene() = default;

		Entity CreateEntity(const std::string& name = std::string(), const TransformComp& transform = TransformComp());
		Triangle2D CreateTriangle2D(const std::string& name = std::string(), const TransformComp& transform = TransformComp(), glm::vec4 color = glm::vec4({ 8.0f, 2.0f, 3.0f, 1.0f }) );
		
		void OnRender();

	private:
		entt::registry m_Registry;

		// Render Element 
		// TODO : Move rendering to another class
		QOpenGLBuffer _positionsBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
		//QOpenGLBuffer _colorsBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
		QOpenGLVertexArrayObject _vao;
		QOpenGLShaderProgram _program;

		FullscreenShader _fullscreenShader;

		friend class Entity;
		friend class Mesh;
		friend class Triangle2D;
	};
