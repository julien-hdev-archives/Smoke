#pragma once

#include <QtGlobal>
#include <QString>
#include <QMessageLogContext>

#include <iostream>

class MessageHandler {
    public:
        static void handler(QtMsgType type, const QMessageLogContext& context, const QString& msg);

    private:
        static const std::string logLevel(QtMsgType type);
};
