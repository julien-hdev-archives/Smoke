#pragma once

#include <QElapsedTimer>

class Time
{
  public:
    static void Initialize();
    static int Elapsed(); // Time since application start, in milliseconds

  private:
    static QElapsedTimer s_timer;
};