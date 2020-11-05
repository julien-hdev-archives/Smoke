#pragma once

#include <QtQuick/QQuickFramebufferObject>
class Scene;

class FrameBufferObject : public QQuickFramebufferObject {
    Q_OBJECT
public:
    explicit FrameBufferObject(QQuickItem* parent=nullptr);
    Renderer* createRenderer() const override;

    Q_INVOKABLE void onMousePress();
    Q_INVOKABLE void onMouseRelease();
    Q_INVOKABLE void onWheelScroll(float delta);

private:
    mutable Scene* _scene;
};
