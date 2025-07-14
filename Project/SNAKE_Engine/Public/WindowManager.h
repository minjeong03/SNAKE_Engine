#pragma once
#include <string>
struct GLFWwindow;
struct EngineContext;

/**
 * @brief Manages window creation, dimensions, and frame-level operations.
 *
 * @details
 * WindowManager is responsible for creating and maintaining the game window using GLFW.
 * It provides access to the GLFWwindow handle and controls screen clearing, buffer swapping,
 * and event polling during the game loop.
 *
 * This class is intended to be used internally by SNAKE_Engine and exposed through EngineContext.
 *
 * @author Jinwoo Choi
 * @date 2025-07-08
 */
class WindowManager
{
    friend class SNAKE_Engine;

public:
    WindowManager():window(nullptr),windowWidth(800),windowHeight(600) {}
    /**
     * @brief Initializes the GLFW window.
     * @return True if window creation was successful.
     */
    bool Init();

    /**
     * @brief Returns a pointer to the GLFW window.
     * @return Pointer to the internal GLFWwindow instance.
     */
   
    [[nodiscard]] GLFWwindow* GetHandle() const { return window; }

    /**
     * @brief Returns the current window width.
     * @return Width in pixels.
     */
    
    [[nodiscard]] int GetWidth() const { return windowWidth; }

    /**
     * @brief Returns the current window height.
     * @return Height in pixels.
     */
   
    [[nodiscard]] int GetHeight() const { return windowHeight; }

    /**
     * @brief Sets the internal window width value (does not resize the actual window).
     * @param width New width in pixels.
     */
    void SetWidth(int width) { this->windowWidth = width; }

    /**
     * @brief Sets the internal window height value (does not resize the actual window).
     * @param height New height in pixels.
     */
    void SetHeight(int height) { this->windowHeight = height; }

    void SetTitle(const std::string& title);

    void Free();


private:
    /**
     * @brief Swaps the front and back buffers (displays the current frame).
     */
    void SwapBuffers() const;

    /**
     * @brief Clears the screen (typically called at the start of each frame).
     */
    void ClearScreen() const;

    /**
     * @brief Polls window and input events (should be called every frame).
     */
    void PollEvents() const;

    /** The GLFW window handle. */
    GLFWwindow* window;

    /** Internal width of the window (in pixels). */
    int windowWidth;

    /** Internal height of the window (in pixels). */
    int windowHeight;
};
