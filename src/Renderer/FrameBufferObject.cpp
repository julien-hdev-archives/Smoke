#include "FrameBufferObject.hpp"
#include "Scene/Scene.hpp"

#include <QtGui/QOpenGLFramebufferObject>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLFunctions>

#include <QtQuick/QQuickWindow>


/****
***** FrameBufferObjectRenderer declaration
****/

class FrameBufferObjectRenderer : public QQuickFramebufferObject::Renderer {
    public:
        FrameBufferObjectRenderer() {
            _scene.initialize();
        }
        ~FrameBufferObjectRenderer() = default;

        void synchronize(QQuickFramebufferObject *item) override {
            _window = item->window();
        }

        void render() override {
            _scene.render();
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


/****
***** FrameBufferObject implementation 
****/

FrameBufferObject::FrameBufferObject(QQuickItem* parent)
    : QQuickFramebufferObject(parent) 
{}

QQuickFramebufferObject::Renderer* FrameBufferObject::createRenderer() const {
    return new FrameBufferObjectRenderer();
}