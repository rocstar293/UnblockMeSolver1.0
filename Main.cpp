#include "Classes.h"

int main()
{

	/*
	std::vector<std::vector<char>> TestArray = {
		{ '-', '-', '-', '-', '-', '-' },
		{ '-', '-', '-', '-', '-', '-' },
		{ '-', '-', '-', '-', '-', '-' },
		{ '-', '-', '-', '-', '-', '-' },
		{ '-', '-', '-', '-', '-', '-' },
		{ '-', '-', '-', '-', '-', '-' }
	};
	*/

	Gameboard Test(TestArray);
	
	Test.Solve();
	return 1;
}
