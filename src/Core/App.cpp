#include "Core/App.hpp"
#include "Core/MessageHandler.hpp"
#include "Renderer/MyQuickFBO.hpp"

#include <QSurfaceFormat>
#include <QtGlobal>
#include <QtQml>
#include <QQuickStyle>
#include <QPalette>


#include "Utility/Time.hpp"

App::App(int &argc, char **argv) : QGuiApplication(argc, argv)
{
    Time::Initialize();

    qmlRegisterType<MyQuickFBO>("SceneGraphRendering", 1, 0, "MyQuickFBO");

    qInstallMessageHandler(MessageHandler::handler);

    setupOpenGLContext();
    setupEngine();
    setupPaletteColor();
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

    const QUrl url(QStringLiteral("qml/main.qml"));
   

    QObject::connect(
        &_engine, &QQmlApplicationEngine::objectCreated, this,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl) QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
        
    _engine.load(url);
}

void
App::setupPaletteColor()
{
    QPalette palette;
    palette.setColor(QPalette::WindowText, QColor(212, 212, 212));
    palette.setColor(QPalette::Button, QColor(45, 45, 45));
    palette.setColor(QPalette::Light, QColor(60, 60, 60));
    palette.setColor(QPalette::Midlight, QColor(52, 52, 52));
    palette.setColor(QPalette::Dark, QColor(30, 30, 30));
    palette.setColor(QPalette::Mid, QColor(37, 37, 37));
    palette.setColor(QPalette::Active, QPalette::Text, QColor(212, 212, 212));
    palette.setColor(QPalette::Disabled, QPalette::Text,
                     QColor(127, 127, 127));
    palette.setColor(QPalette::Inactive, QPalette::Text,
                     QColor(212, 212, 212));
    palette.setColor(QPalette::BrightText, QColor(240, 240, 240));
    palette.setColor(QPalette::ButtonText, QColor(212, 212, 212));
    palette.setColor(QPalette::Base, QColor(60, 60, 60));
    palette.setColor(QPalette::Window, QColor(37, 37, 37));
    palette.setColor(QPalette::Shadow, QColor(0, 0, 0));
    palette.setColor(QPalette::Highlight, QColor(38, 79, 120));
    palette.setColor(QPalette::HighlightedText, QColor(240, 240, 240));
    palette.setColor(QPalette::Link, QColor(120, 120, 255));
    palette.setColor(QPalette::LinkVisited, QColor(255, 0, 255));
    palette.setColor(QPalette::AlternateBase, QColor(45, 45, 45));
    palette.setColor(QPalette::NoRole, QColor(0, 0, 0));
    palette.setColor(QPalette::ToolTipBase, QColor(255, 255, 220));
    palette.setColor(QPalette::ToolTipText, QColor(0, 0, 0));

    this->setPalette(palette);
}

