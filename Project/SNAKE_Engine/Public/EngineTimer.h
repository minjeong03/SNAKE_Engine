#pragma once

/**
 * @brief Tracks frame timing and calculates delta time and FPS.
 *
 * Provides per-frame timing data such as delta time and frame rate updates.
 * Intended to be called once per frame from the game loop.
 *
 * Usage:
 * @code
 * EngineTimer timer;
 * timer.Start();
 * while (running)
 * {
 *     float dt = timer.Tick();
 *     if (timer.ShouldUpdateFPS(fps))
 *     {
 *         std::cout << "FPS: " << fps << std::endl;
 *     }
 * }
 * @endcode
 */
struct EngineTimer
{
    float lastTime = 0.0f;   ///< Time of the last Tick() call.
    float fpsTimer = 0.0f;   ///< Accumulated time for FPS measurement.
    int frameCount = 0;      ///< Frame counter for FPS calculation.

    /**
     * @brief Initializes timing state using the current time.
     *
     * Typically called once before the main loop starts.
     */
    void Start();

    /**
     * @brief Updates internal timing and returns delta time since last Tick().
     *
     * Should be called once per frame.
     *
     * @return Time elapsed since the previous Tick(), in seconds.
     */
    [[nodiscard]] float Tick();

    /**
     * @brief Returns true if it's time to update the FPS value.
     *
     * Usually returns true every 1 second. Also writes the calculated FPS to the output parameter.
     *
     * @param outFPS Output reference for the calculated frames per second.
     * @return True if FPS should be updated this frame.
     */
    [[nodiscard]] bool ShouldUpdateFPS(float& outFPS);
};
