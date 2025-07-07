#include <iostream>
#include "test.h"
int main()
{
	Test t = Test();
	std::cout << "main test: "<< t.key << std::endl;
	return 0;
}