#include <iostream>

#include "Debug.h"
#include "MainMenu.h"
#include "Engine.h"

int main(int argc, char* argv[])
{
	SNAKE_Engine snakeEngine;

	if (argc == 3)
	{
		snakeEngine.Init(atoi(argv[1]),  atoi(argv[2]));
	}
	else if (argc == 1)
	{
		snakeEngine.Init(800,600);
	}
	else
	{
		SNAKE_ERR("Too many arguments for input");
		return -1;
	}


	snakeEngine.GetEngineContext().stateManager->ChangeState(std::make_unique<MainMenu>());

	snakeEngine.Run();

	return 0;
}