#pragma once

#include <QMessageLogContext>
#include <QString>
#include <QtGlobal>

#include <iostream>

class MessageHandler
{
  public:
    static void handler (QtMsgType type, const QMessageLogContext &context,
                         const QString &msg);

  private:
    static const std::string logLevel (QtMsgType type);
};
