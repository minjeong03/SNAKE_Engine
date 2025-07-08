#include <iostream>
#include "test.h"

int main(int argc, char* argv[])
{
	Test t = Test();
	if (argc == 3)
	{
		t.Init(atoi(argv[1]),atoi(argv[2]));
	}
	else if (argc == 1)
	{
		t.Init(800, 600);
	}
	else
	{
		std::cout << "ERROR: \n\tToo less or much CMD-line arguments \n\t" << __FUNCTION__ << " " << __LINE__ << std::endl;
		return -1;
	}
	
	while (1)
	{
		t.Update();
	}
	std::cout << "main test: "<< t.key << std::endl;
	return 0;
}