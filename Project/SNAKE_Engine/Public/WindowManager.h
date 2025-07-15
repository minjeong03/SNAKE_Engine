#pragma once
#include <string>
class SNAKE_Engine;
struct GLFWwindow;
struct EngineContext;

/**
 * @brief Handles creation and control of the main GLFW application window.
 *
 * Provides window initialization, resizing, title setting, buffer swapping,
 * screen clearing, and event polling. Internally used by SNAKE_Engine.
 */
class WindowManager
{
    friend SNAKE_Engine;
    friend void framebuffer_size_callback(GLFWwindow*, int, int);
public:
    /**
     * @brief Constructs the WindowManager with default dimensions (800x600).
     */
    WindowManager() :window(nullptr), windowWidth(800), windowHeight(600) {}

    /**
     * @brief Returns the raw pointer to the internal GLFWwindow.
     * @return Pointer to GLFWwindow instance.
     */
    [[nodiscard]] GLFWwindow* GetHandle() const { return window; }

    /**
     * @brief Gets the current window width in pixels.
     * @return Window width.
     */
    [[nodiscard]] int GetWidth() const { return windowWidth; }

    /**
     * @brief Gets the current window height in pixels.
     * @return Window height.
     */
    [[nodiscard]] int GetHeight() const { return windowHeight; }

    /**
     * @brief Resizes the actual GLFW window and updates internal dimensions.
     * @param width New width in pixels.
     * @param height New height in pixels.
     */
    void Resize(int width, int height);

    /**
     * @brief Sets the title displayed on the GLFW window.
     * @param title Title string to set.
     */
    void SetTitle(const std::string& title) const;

private:
    /**
     * @brief Initializes the GLFW window and OpenGL context.
     *
     * Loads OpenGL context, sets viewport, registers callbacks,
     * and associates engine reference.
     *
     * @param _windowWidth Initial window width.
     * @param _windowHeight Initial window height.
     * @param engine Reference to the SNAKE_Engine.
     * @return True if initialization is successful.
     */
    bool Init(int _windowWidth, int _windowHeight, SNAKE_Engine& engine);

    /**
     * @brief Updates the stored internal width value.
     * @param width New width in pixels.
     */
    void SetWidth(int width) { this->windowWidth = width; }

    /**
     * @brief Updates the stored internal height value.
     * @param height New height in pixels.
     */
    void SetHeight(int height) { this->windowHeight = height; }

    /**
     * @brief Swaps front and back buffers to present the current frame.
     */
    void SwapBuffers() const;

    /**
     * @brief Clears the screen color buffer.
     */
    void ClearScreen() const;

    /**
     * @brief Polls window system and input events.
     */
    void PollEvents() const;

    /**
     * @brief Destroys the GLFW window and frees associated resources.
     */
    void Free() const;

    GLFWwindow* window;
    int windowWidth;
    int windowHeight;
};
