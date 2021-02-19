#include "MessageHandler.hpp"

const std::string
MessageHandler::logLevel(QtMsgType type)
{
    switch (type)
    {
    case QtDebugMsg:
        return "[DEBUG] ";
    case QtInfoMsg:
        return "[INFO] ";
    case QtWarningMsg:
        return "[WARNING] ";
    case QtCriticalMsg:
        return "[CRITICAL] ";
    case QtFatalMsg:
        return "[FATAL] ";
    default:
        return "[] ";
    }
}

void
MessageHandler::handler(QtMsgType type, const QMessageLogContext &context,
                        const QString &msg)
{
    const std::string logLevel = MessageHandler::logLevel(type);
    const std::string file = context.file ? context.file : "";
    const std::string function = context.function ? context.function : "";
    const std::string localMsg = msg.toStdString();

    std::cerr << logLevel << localMsg << " (" << file << ":" << context.line
              << ", " << function << ")" << std::endl;
}
