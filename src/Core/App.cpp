#include "Core/App.hpp"
#include "Renderer/FrameBufferObject.hpp"
#include "Core/MessageHandler.hpp"

#include <QtGlobal>
#include <QtQml>
#include <QSurfaceFormat>

App::App(int &argc, char **argv)
    : QGuiApplication(argc, argv) 
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    qmlRegisterType<FrameBufferObject>("SceneGraphRendering", 1, 0, "FrameBufferObject");

    qInstallMessageHandler(MessageHandler::handler);

    setupOpenGLContext();
    setupEngine();
}

void App::setupOpenGLContext() const {
    QSurfaceFormat format;

    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setSamples(4);
    format.setSwapInterval(0);

    QSurfaceFormat::setDefaultFormat(format);
}

void App::setupEngine() {
    const QUrl url(QStringLiteral("qml/main.qml"));

    QObject::connect(&_engine, &QQmlApplicationEngine::objectCreated,
                     this, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    _engine.load(url);
}