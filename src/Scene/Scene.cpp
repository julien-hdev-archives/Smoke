#include "Scene.hpp"

#include "Scene/Entity.hpp"
#include "Scene/Mesh.hpp"
#include "Scene/Triangle2D.hpp"

#include <glm/glm.hpp>
#include <QtGui/QOpenGLFunctions>

#include "Renderer/Renderer.hpp"

#include "Utility/File.hpp"

	Scene::Scene()
        : _fullscreenShader(MyFile::LoadToString("shaders/fullscreen2.fs.glsl"))
	{
        // Render configuration 
        // TODO : Move rendering to another class
		if (!_vao.create())
			qFatal("Unable to create VAO.");

        _vao.bind();
      
        if (!_positionsBuffer.create())
            qFatal("Unable to create positions buffer.");
        _positionsBuffer.bind();
        _positionsBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);

        /*
        QVector<QVector3D> colors;
        colors.push_back(QVector3D(1.f, 0.f, 0.f));
        colors.push_back(QVector3D(0.f, 1.f, 0.f));
        colors.push_back(QVector3D(0.f, 0.f, 1.f));

        if (!_colorsBuffer.create())
            qFatal("Unable to create colors buffer.");
        _colorsBuffer.bind();
        _colorsBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
        _colorsBuffer.allocate(colors.constData(), colors.size() * sizeof(QVector3D));*/


        if (!_program.create())
            qFatal("Unable to create shader program.");
        if (!_program.addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/triangle.vs.glsl"))
            qFatal("Vertex shader compilation failed.");
        if (!_program.addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/triangle.fs.glsl"))
            qFatal("Fragment shader compilation failed.");
        if (!_program.link())
            qFatal("Shader program not linked.");

        _program.bind();

        const int VERTEX_ATTR_POS = 0;
        _positionsBuffer.bind();
        _program.enableAttributeArray(VERTEX_ATTR_POS);
        _program.setAttributeArray(VERTEX_ATTR_POS, GL_FLOAT, 0, 4);

        /*
        const int VERTEX_ATTR_COL = 1;
        _colorsBuffer.bind();
        _program.enableAttributeArray(VERTEX_ATTR_COL);
        _program.setAttributeArray(VERTEX_ATTR_COL, GL_FLOAT, 0, 3);*/

        _vao.release();

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
        
        //std::vector<glm::vec4> vertices;
        //
        //// Recup all vertices to render.
		//auto group = m_Registry.group<MeshComp>(entt::get<TransformComp>);
		//for (auto entity : group)
		//{
		//	auto [mesh, transform] = group.get<MeshComp, TransformComp>(entity);
        //
        //    for (uint32_t i = 0; i < mesh.VertexCount; i++)
        //    {
        //        vertices.push_back(transform.Transform * mesh.Vertices[i]);
        //    }
		//}
        //
        ////Reallocate the VertexBuffer
        //_positionsBuffer.bind();
        //_positionsBuffer.allocate(vertices.data(), vertices.size() * sizeof(glm::vec4));
        
        QOpenGLFunctions* functions = QOpenGLContext::currentContext()->functions();
        functions->glClearColor(0.f, 0.f, 0.f, 1.f);
        functions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        // Render
        //_vao.bind();
        //_program.bind();
        //functions->glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        //_vao.release();
        _fullscreenShader.draw();
	}

