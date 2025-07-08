#pragma once

struct GLFWwindow;
class Test
{
public:
	Test()
	{
	}
	void Init(const unsigned int& width, const unsigned int& height);

	void Update();

	void Destroy();

	GLFWwindow* window;

	int key = 5;
};