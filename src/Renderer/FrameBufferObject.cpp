#include "FrameBufferObject.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Triangle2D.hpp"

#include <QtGui/QOpenGLFramebufferObject>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLFunctions>

#include <QtQuick/QQuickWindow>

class FrameBufferObjectRenderer : public QQuickFramebufferObject::Renderer {
    public:
        FrameBufferObjectRenderer() {
            _scene.CreateTriangle2D(); //TODO : Move creation to another class
        }
        ~FrameBufferObjectRenderer() = default;

        void synchronize(QQuickFramebufferObject *item) override {
            _window = item->window();
        }

        void render() override {
            _scene.OnRender();
            update();
            _window->resetOpenGLState();
        }

        QOpenGLFramebufferObject* createFramebufferObject(const QSize& size) override {
            QOpenGLFramebufferObjectFormat format;
            format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
            format.setSamples(4);
            return new QOpenGLFramebufferObject(size, format);
        }
    
    private:
        Scene _scene;
        QQuickWindow* _window;
};

FrameBufferObject::FrameBufferObject(QQuickItem* parent)
    : QQuickFramebufferObject(parent) 
{}

QQuickFramebufferObject::Renderer* FrameBufferObject::createRenderer() const {
    return new FrameBufferObjectRenderer();
}