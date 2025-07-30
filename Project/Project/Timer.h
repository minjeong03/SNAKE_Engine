#pragma once

class Timer
{
public:
    void Start(float time);
    void Update(float dt);
    bool IsTimedOut() const;
    bool IsStarted() const;

    float GetProgressPercentage() const;

private:
    float time = 0;
    float elapsed = 0;
    bool started = false;
};
