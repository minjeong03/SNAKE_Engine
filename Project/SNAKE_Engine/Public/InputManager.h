#pragma once
#include <bitset>

class SNAKE_Engine;
struct GLFWwindow;

/**
 * @brief Handles per-frame keyboard and mouse input polling using GLFW.
 *
 * @details
 * This class manages input state by caching current and previous frames' key and mouse button statuses.
 * It supports querying for real-time key/mouse down, press, and release events, as well as tracking the cursor position.
 *
 * InputManager must be initialized with a valid GLFW window before use.
 *
 * @code
 * if (engineContext.inputManager->IsKeyPressed(KEY_SPACE))
 * {
 *     player->Jump();
 * }
 * @endcode
 */
class InputManager
{
    friend SNAKE_Engine;

public:

    InputManager() :window(nullptr), mouseX(0.0), mouseY(0.0) {}

    /**
     * @brief Checks if a specific key is currently being held down.
     *
     * @details
     * Returns true if the specified key is currently held during this frame.
     *
     * @param key Integer GLFW key code (see InputKey enum).
     * @return True if key is currently down.
     */
    [[nodiscard]] bool IsKeyDown(int key) const;

    /**
     * @brief Checks if a specific key was pressed during this frame.
     *
     * @details
     * Returns true if the key was not held last frame but is held this frame.
     *
     * @param key Integer GLFW key code.
     * @return True if key was just pressed.
     *
     * @code
     * if (inputManager->IsKeyPressed(KEY_ENTER))
     * {
     *     ConfirmSelection();
     * }
     * @endcode
     */
    [[nodiscard]] bool IsKeyPressed(int key) const;

    /**
     * @brief Checks if a specific key was released during this frame.
     *
     * @details
     * Returns true if the key was held last frame but is not held now.
     *
     * @param key Integer GLFW key code.
     * @return True if key was just released.
     */
    [[nodiscard]] bool IsKeyReleased(int key) const;

    /**
     * @brief Checks if a mouse button is currently being held down.
     *
     * @details
     * Returns true if the specified mouse button is held this frame.
     *
     * @param button Integer GLFW mouse button code (see InputMouseButton enum).
     * @return True if button is down.
     */
    [[nodiscard]] bool IsMouseButtonDown(int button) const;

    /**
     * @brief Checks if a mouse button was just pressed this frame.
     *
     * @details
     * Returns true if the button was not held last frame but is now pressed.
     *
     * @param button Integer mouse button code.
     * @return True if button was just pressed.
     *
     * @code
     * if (inputManager->IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
     * {
     *     SelectObject();
     * }
     * @endcode
     */
    [[nodiscard]] bool IsMouseButtonPressed(int button) const;

    /**
     * @brief Checks if a mouse button was just released this frame.
     *
     * @details
     * Returns true if the button was pressed last frame and is now released.
     *
     * @param button Integer mouse button code.
     * @return True if button was just released.
     */
    [[nodiscard]] bool IsMouseButtonReleased(int button) const;

    /**
     * @brief Retrieves the current X position of the mouse cursor in pixels.
     *
     * @return X position of the mouse in window coordinates.
     */
    [[nodiscard]] double GetMouseX() const;

    /**
     * @brief Retrieves the current Y position of the mouse cursor in pixels.
     *
     * @return Y position of the mouse in window coordinates.
     */
    [[nodiscard]] double GetMouseY() const;

private:
    /**
     * @brief Initializes the input manager with a GLFW window.
     *
     * @details
     * This function must be called exactly once before querying input.
     * It is intended to be called only by the engine during startup.
     *
     * @param _window A pointer to the GLFW window used for input polling.
     */
    void Init(GLFWwindow* _window);

    /**
     * @brief Updates the input state for the current frame.
     *
     * @details
     * This function should be called once every frame.
     * It captures the current keyboard and mouse states and computes the transition logic.
     * Intended for internal use by the engine loop.
     */
    void Update();

    GLFWwindow* window;

    static constexpr int MAX_KEYS = 349;
    static constexpr int MAX_MOUSE_BUTTONS = 9;

    std::bitset<MAX_KEYS> currentKeyState;
    std::bitset<MAX_KEYS> previousKeyState;
    std::bitset<MAX_MOUSE_BUTTONS> currentMouseState;
    std::bitset<MAX_MOUSE_BUTTONS> previousMouseState;

    double mouseX;
    double mouseY;
};


/**
 * @brief Defines keyboard key codes used by the input system.
 *
 * @details
 * These values are directly mapped to GLFW key codes.
 * Use them as parameters in InputManager methods such as IsKeyDown(), IsKeyPressed(), and IsKeyReleased().
 *
 * @code
 * if (inputManager->IsKeyPressed(KEY_ESCAPE))
 * {
 *     engine->RequestQuit();
 * }
 * @endcode
 */

enum InputKey
{
    KEY_UNKNOWN = -1,
    KEY_SPACE = 32,
    KEY_APOSTROPHE = 39,  /* ' */
    KEY_COMMA = 44,  /* , */
    KEY_MINUS = 45,  /* - */
    KEY_PERIOD = 46,  /* . */
    KEY_SLASH = 47,  /* / */
    KEY_0 = 48,
    KEY_1 = 49,
    KEY_2 = 50,
    KEY_3 = 51,
    KEY_4 = 52,
    KEY_5 = 53,
    KEY_6 = 54,
    KEY_7 = 55,
    KEY_8 = 56,
    KEY_9 = 57,
    KEY_SEMICOLON = 59,  /* ; */
    KEY_EQUAL = 61,  /* = */
    KEY_A = 65,
    KEY_B = 66,
    KEY_C = 67,
    KEY_D = 68,
    KEY_E = 69,
    KEY_F = 70,
    KEY_G = 71,
    KEY_H = 72,
    KEY_I = 73,
    KEY_J = 74,
    KEY_K = 75,
    KEY_L = 76,
    KEY_M = 77,
    KEY_N = 78,
    KEY_O = 79,
    KEY_P = 80,
    KEY_Q = 81,
    KEY_R = 82,
    KEY_S = 83,
    KEY_T = 84,
    KEY_U = 85,
    KEY_V = 86,
    KEY_W = 87,
    KEY_X = 88,
    KEY_Y = 89,
    KEY_Z = 90,
    KEY_LEFT_BRACKET = 91,  /* [ */
    KEY_BACKSLASH = 92,  /* \ */
    KEY_RIGHT_BRACKET = 93,  /* ] */
    KEY_GRAVE_ACCENT = 96,  /* ` */

    KEY_ESCAPE = 256,
    KEY_ENTER = 257,
    KEY_TAB = 258,
    KEY_BACKSPACE = 259,
    KEY_INSERT = 260,
    KEY_DELETE = 261,
    KEY_RIGHT = 262,
    KEY_LEFT = 263,
    KEY_DOWN = 264,
    KEY_UP = 265,
    KEY_PAGE_UP = 266,
    KEY_PAGE_DOWN = 267,
    KEY_HOME = 268,
    KEY_END = 269,
    KEY_CAPS_LOCK = 280,
    KEY_SCROLL_LOCK = 281,
    KEY_NUM_LOCK = 282,
    KEY_PRINT_SCREEN = 283,
    KEY_PAUSE = 284,

    KEY_F1 = 290,
    KEY_F2 = 291,
    KEY_F3 = 292,
    KEY_F4 = 293,
    KEY_F5 = 294,
    KEY_F6 = 295,
    KEY_F7 = 296,
    KEY_F8 = 297,
    KEY_F9 = 298,
    KEY_F10 = 299,
    KEY_F11 = 300,
    KEY_F12 = 301,

    KEY_LEFT_SHIFT = 340,
    KEY_LEFT_CONTROL = 341,
    KEY_LEFT_ALT = 342,
    KEY_LEFT_SUPER = 343,
    KEY_RIGHT_SHIFT = 344,
    KEY_RIGHT_CONTROL = 345,
    KEY_RIGHT_ALT = 346,
    KEY_RIGHT_SUPER = 347,

    KEY_MENU = 348
};
/**
 * @brief Defines mouse button codes used by the input system.
 *
 * @details
 * These values correspond to GLFW mouse button constants.
 * Use them in InputManager methods such as IsMouseButtonDown(), IsMouseButtonPressed(), and IsMouseButtonReleased().
 *
 * @code
 * if (inputManager->IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
 * {
 *     SelectObject();
 * }
 * @endcode
 */

enum InputMouseButton
{
    MOUSE_BUTTON_1 = 0,
    MOUSE_BUTTON_LEFT = MOUSE_BUTTON_1,  // alias
    MOUSE_BUTTON_2 = 1,
    MOUSE_BUTTON_RIGHT = MOUSE_BUTTON_2, // alias
    MOUSE_BUTTON_3 = 2,
    MOUSE_BUTTON_MIDDLE = MOUSE_BUTTON_3, // alias
    MOUSE_BUTTON_4 = 3,
    MOUSE_BUTTON_5 = 4,
    MOUSE_BUTTON_6 = 5,
    MOUSE_BUTTON_7 = 6,
    MOUSE_BUTTON_8 = 7,

    MOUSE_BUTTON_LAST = MOUSE_BUTTON_8
};
