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

	std::vector<std::vector<char>> TestArray = {
		{ '-', 'A', 'B', 'B', 'C', 'C' },
		{ '-', 'A', '-', 'D', 'E', 'E' },
		{ '#', '#', '-', 'D', 'F', '-' },
		{ 'G', 'H', 'H', 'H', 'F', 'I' },
		{ 'G', '-', 'J', '-', 'F', 'I' },
		{ 'G', '-', 'J', 'K', 'K', 'I' }
	};


	Gameboard Test(TestArray);
	Test.Solve();
	return 1;
}