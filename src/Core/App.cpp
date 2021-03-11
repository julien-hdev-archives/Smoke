#include "Core/App.hpp"
#include "Core/MessageHandler.hpp"
#include "Renderer/MyQuickFBO.hpp"
#include "UI/Tree/MyTreeNode.hpp"
#include "UI/Tree/MyTreeModel.hpp"

#include <QtGlobal>
#include <QtQml>
#include <QQuickStyle>
#include <QPalette>
#include <QQmlEngine>
#include <QIcon>
#include "Simulation/glwrappers.hpp"

#include "Utility/Time.hpp"

App::App(int &argc, char **argv) : QGuiApplication(argc, argv)
{
    Time::Initialize();

    qmlRegisterType<MyQuickFBO>("SceneGraphRendering", 1, 0, "MyQuickFBO");

    qmlRegisterType<MyTreeModel>("MyTree", 1, 0, "TreeModel");
    qmlRegisterType<MyTreeNode>("MyTree", 1, 0, "TreeElement");


    qInstallMessageHandler(MessageHandler::handler);

    setupOpenGLContext();
    setupEngine();
}

void
App::setupOpenGLContext() const
{
    QSurfaceFormat format;

    format.setMajorVersion(4);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSamples(1);
    format.setSwapInterval(1);

    QSurfaceFormat::setDefaultFormat(format);
}

void
App::setupEngine()
{
    _engine.addImportPath(QStringLiteral("qml"));
    const QUrl url(QStringLiteral("qml/main.qml"));
   
    QQuickStyle::setStyle("Material");
    this->setWindowIcon(QIcon("qml/images/smoke_icon.png"));

    QObject::connect(
        &_engine, &QQmlApplicationEngine::objectCreated, this,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl) QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
        
    _engine.load(url);
}


