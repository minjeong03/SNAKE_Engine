#include <iostream>

#include "MainMenu.h"
#include "SNAKE_Engine.h"
#include "StateManager.h"

int main(int argc, char* argv[])
{
	SNAKE_engine snakeEngine;


	if (argc == 3)
	{
		snakeEngine.Init(argc, argv);
	}
	else if (argc == 1)
	{
		snakeEngine.Init();
	}
	else
	{
		std::cout << "ERROR: \n\tToo less or much CMD-line arguments \n\t" << __FUNCTION__ << " " << __LINE__ << "\n";
		return -1;
	}


	snakeEngine.GetStateManager().ChangeState(std::make_unique<MainMenu>());

	snakeEngine.Run();

	return 0;
}