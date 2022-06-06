
#include <iostream>
#include <vector>
#include <unordered_map>
#include <iterator>
#include <chrono>

#pragma once

class Gameboard;
class Block;


class Gameboard
{

public:


	// PROPERTIES

	std::vector<std::vector<char>> data;											// 2d vector array of chars representing board contents

	std::unordered_map<char, Block> blocks;											// Hash map of block id's with cooresponding block

	std::unordered_map<std::string, std::pair<int, std::string>> gamestate_history; // hash map of game states paired with cooresponding movecount and parent state

	int movecount;																	// counter for total moves


	// FUNCTIONS

	Gameboard(std::vector<std::vector<char>>);										// initializer for Gameboard using a given 2d char vector array

	void UpdateData();																// updates the data vector to match contents of the blocks vector

	void UpdateBlocks();															// updates the blocks vector to match contents of the data vector

	std::vector<std::pair<char, std::vector<int>>> GetAllMoves();					// parses Board and returns list of moves for each block

	void MoveBlock(std::pair<char, int>);											// moves block at char by given signed integer value

	std::pair<std::string, std::string> SaveState(std::string a_parent_state);		// saves current board data and appends to gamestate_history

	void GoToState(std::string a_state);											// sets the gameboard to state represented by a string

	bool QueryDuplicateMove();														// returns true if gamestate_history already contains existsing state 

	bool QuerySolved();																// traces horizontal movement of escape(#) block; returns true if exit is possible

	void Solve(bool debug = false);													// breadth first search algorithm; traverses all possible moves until first solution state is found

	void PrintBoard();																// prints visual representation of board state

	void PrintSolution(std::pair<std::string, std::string> _solved_state);			// if solution exists, traverses parents of each state from solution until it reaches first state,
																					// appends these states to list, reverses the list, and outputs each move required to get to solution

};

class Block
{

public:


	// PROPERTIES

	char id;																		// unique identifier for each block

	std::vector<std::pair<int, int>> coords;										// x, y values of each index of block

	bool is_horizontal;																// persistant value to assist with block movement logic


	//FUNCTIONS

	Block();																		// default Initializer for block
	
	Block(char a_id);																// initializes block with an ID
	
	void Calculate();																// Sets member data from block's coordinates
	
	std::vector<int> GetMoves(std::vector<std::vector<char>> _data);				// returns list of available moves using Gameboard data
};