#include "Timer.h"

void Timer::Start(float time)
{
    this->time = time;
    elapsed = 0;
    started = true;
}

void Timer::Update(float dt)
{
    elapsed += dt;
}

bool Timer::IsTimedOut() const
{
    return started && elapsed >= time;
}

bool Timer::IsStarted() const
{
    return started;
}

float Timer::GetProgressPercentage() const
{
    return elapsed / time;
}
