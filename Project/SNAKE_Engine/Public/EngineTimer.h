#pragma once

struct EngineTimer
{
    float lastTime = 0.0f;
    float fpsTimer = 0.0f;
    int frameCount = 0;

    void Start();

    [[nodiscard]] float Tick();

    [[nodiscard]] bool ShouldUpdateFPS(float& outFPS);
};
