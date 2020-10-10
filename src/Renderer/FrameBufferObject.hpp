#pragma once

#include <QtQuick/QQuickFramebufferObject>

class FrameBufferObject : public QQuickFramebufferObject {
    Q_OBJECT
public:
    explicit FrameBufferObject(QQuickItem* parent=nullptr);
    Renderer* createRenderer() const override;
};
