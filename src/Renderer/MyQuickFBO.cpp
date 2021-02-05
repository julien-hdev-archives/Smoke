#pragma once

#include "MyQuickFBO.hpp"
#include "MyQuickFBORenderer.hpp"

MyQuickFBO::MyQuickFBO(QQuickItem* parent)
    : QQuickFramebufferObject(parent)
{}

QQuickFramebufferObject::Renderer* MyQuickFBO::createRenderer() const {
    return new MyQuickFBORenderer();
}

void MyQuickFBO::onMousePress() {
    _camera.onWheelDown();
}
void MyQuickFBO::onMouseRelease() {
    _camera.onWheelUp();
}
void MyQuickFBO::onWheelScroll(float delta) {
    _camera.onWheelScroll(delta);
    update();
}