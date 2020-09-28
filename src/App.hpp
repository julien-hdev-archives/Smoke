#ifndef APP_HPP
#define APP_HPP
#pragma once

#include <QGuiApplication>
#include <QQmlApplicationEngine>

class App : public QGuiApplication {
    public:
        App(int &argc, char **argv);
        ~App() = default;

    private:
        void setupOpenGLContext() const;
        void setupEngine();

    private:
        QQmlApplicationEngine _engine;
};

#endif