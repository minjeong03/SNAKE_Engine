#pragma once

struct GLFWwindow;
struct EngineContext;
/**
 * @brief 
 * @details
 * @author Jinwoo Choi
 * @date 2025-07-08
 */
class WindowManager
{
public:
	bool Init();

	GLFWwindow* GetHandle() const { return window; }
	int GetWidth() const { return windowWidth; }
	int GetHeight() const { return windowHeight; }
	void SetWidth(int width)  { this->windowWidth = width; }
	void SetHeight(int height)  { this->windowHeight = height; }
	void SwapBuffers() const;
	void ClearScreen() const;
	void PollEvents() const;
private:
	GLFWwindow* window = nullptr;
	int windowWidth = 800;
	int windowHeight = 600;
};
