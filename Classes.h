#include <iostream>
#include <vector>
#include <unordered_map>
#include <iterator>

#pragma once

class Gameboard;
class Block;


class Gameboard
{

public:
	
	// PROPERTIES
	
	std::vector<std::vector<char>> data; // 2d vector array of chars representing board contents

	std::vector<						// list
		std::pair<						// of pairs
			Block, std::vector<			// of blocks with list
				std::pair<int, int>>>>	// of it's paired coordinates
					blocks;
	
	int movecount; // counter for total moves
	
	std::unordered_map<std::string, std::pair<int, std::string>> gamestate_history; // hash map of game states paired with cooresponding movecount

	// FUNCTIONS

	Gameboard(std::vector<std::vector<char>>);							// initializer for Gameboard using a given 2d char vector array
	void UpdateData();
	void UpdateBlocks();
	std::vector<std::pair<char, std::vector<int>>> GetAllMoves();		// Parses Board and returns list of moves for each block
	void MoveBlock(std::pair<char, int>);
	std::pair<std::string, std::string> SaveState(std::string parent);	// saves current board data and appends to gamestate_history
	void GoToState(std::string _state);
	bool QuerySolved();
	bool QueryDuplicateMove();
	void Solve(bool debug = false);
	void PrintBoard();
	void PrintSolution(std::pair<std::string, std::string> _solved_state);

};

class Block
{

public:
	
	// PROPERTIES

	char id;					// unique char representing each block on the board
	bool is_horizontal;			
	
	//FUNCTIONS

	Block(char _id);
	void Calculate(std::vector<std::pair<int, int>> _coords);
	std::pair<char, std::vector<int>> GetMoves(std::vector<std::pair<int, int>> _coords, std::vector<std::vector<char>> _data);

};