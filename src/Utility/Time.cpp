#include "Time.hpp"

QElapsedTimer Time::s_timer;

void
Time::Initialize ()
{
    s_timer.start ();
}

int
Time::Elapsed ()
{
    return s_timer.elapsed ();
}