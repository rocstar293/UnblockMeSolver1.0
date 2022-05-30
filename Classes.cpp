#include "Classes.h"

Gameboard::Gameboard(std::vector<std::vector<char>> _data)
{
	data = _data;

	UpdateBlocks();

	for (int block_cnt = 0; block_cnt < blocks.size(); block_cnt++)
		blocks[block_cnt].first.Calculate(blocks[block_cnt].second);
}

void Gameboard::UpdateData()
{
	for (int c = 0; c < data.size(); c++)
	{
		std::fill(data[c].begin(), data[c].end(), '-');
	}

	for (int i = 0; i < blocks.size(); i++)
	{
		for (int j = 0; j < blocks[i].second.size(); j++)
		{
			data[blocks[i].second[j].second][blocks[i].second[j].first] = blocks[i].first.id;
		}
	}
}

void Gameboard::UpdateBlocks()
{
	blocks.clear();
	
	for (int c = 0; c < data.size(); c++) {
		for (int r = 0; r < data[0].size(); r++)
		{
			if (data[c][r] == '-')
				continue;

			std::pair<int, int> coord = { r, c };
			bool found = false;

			for (int i = 0; i < blocks.size(); i++)
			{
				if (blocks[i].first.id == data[c][r])
				{
					blocks[i].second.push_back(coord);
					found = true;
					break;
				}
			}
			if (found == false)
			{
				blocks.push_back(std::pair<Block, std::vector<std::pair<int, int>>>(Block(data[c][r]), std::vector<std::pair<int, int>>({ coord })));
			}
		}
	}

	for (int block_cnt = 0; block_cnt < blocks.size(); block_cnt++)
		blocks[block_cnt].first.Calculate(blocks[block_cnt].second);
}

std::vector<std::pair<char, std::vector<int>>> Gameboard::GetAllMoves()
{
	std::vector<std::pair<char, std::vector<int>>> _movelist;

	for (int i = 0; i < blocks.size(); i++)
	{
		_movelist.push_back(blocks[i].first.GetMoves(blocks[i].second, data));
	}
	
	return _movelist;
}

void Gameboard::MoveBlock(std::pair<char, int> _move)
{
	for (int i = 0; i < blocks.size(); i++)
	{
		if (blocks[i].first.id == _move.first)
		{	

			if (blocks[i].first.is_horizontal)
			{
				for (int j = 0; j < blocks[i].second.size(); j++)
				{
					blocks[i].second[j] = std::pair<int, int>{ blocks[i].second[j].first + _move.second, blocks[i].second[j].second };
				}
			}
			else
			{
				for (int j = 0; j < blocks[i].second.size(); j++)
				{
					blocks[i].second[j] = std::pair<int, int>{ blocks[i].second[j].first, blocks[i].second[j].second + _move.second };
				}
			}
		}
	}
	UpdateData();
}

std::pair<std::string, std::string> Gameboard::SaveState(std::string _parent_state)
{
	std::string gamestate = "";
	
	for (int c = 0; c < data.size(); c++) {
		for (int r = 0; r < data[0].size(); r++)
		{
			gamestate += data[c][r];
		}
	}
	gamestate_history[gamestate] = { movecount, _parent_state };
	
	return { gamestate, _parent_state };
}

void Gameboard::GoToState(std::string _state)
{
	int save_iter = 0;
	for (int c = 0; c < data.size(); c++) {
		for (int r = 0; r < data[0].size(); r++)
		{
			data[c][r] = _state[save_iter];
			save_iter++;
		}
	}
	UpdateBlocks();
}

void Gameboard::Solve(bool debug)
{
	std::pair<std::string, std::string> prev_state = SaveState("First");

	while (true)
	{
		movecount++;
		std::unordered_map<std::string, std::pair<int, std::string>>::iterator gamestate_it = gamestate_history.begin();
		
		if (QuerySolved())
		{
			std::cout << "Already Solved!" << std::endl;
			return;
		}
		
		int new_gamestates = 0;
		
		while (gamestate_it != gamestate_history.end())
		{
			if (gamestate_it->second.first == movecount - 1)
			{
				GoToState(gamestate_it->first);
				prev_state = { gamestate_it->first , gamestate_it->first };
				std::vector<std::pair<char, std::vector<int>>> _movelist = GetAllMoves();

				for (int i = 0; i < _movelist.size(); i++)
				{
					for (int j = 0; j < _movelist[i].second.size(); j++)
					{
						MoveBlock(std::pair<char, int> { _movelist[i].first, _movelist[i].second[j] });
						if (QueryDuplicateMove() == false)
						{
							std::pair<std::string, std::string> current_state = SaveState(prev_state.second);

							if (debug == true)
							{
								std::cout << " -------- " << std::endl;
								PrintBoard();
							}

							new_gamestates++;


							if (QuerySolved())
							{
								std::cout << "======== Solution ========" << std::endl;
								PrintSolution(current_state);
								return;
							}
						}
						GoToState(prev_state.first);
					}
				}
			}
			gamestate_it++;
		}
		if (new_gamestates == 0)
		{
			std::cout << "=== No Solution ===" << std::endl;
			return;
		}
	}
}

bool Gameboard::QuerySolved()
{
	bool is_free = true;

	for (int i = 0; i < blocks.size(); i++)
	{
		if (blocks[i].first.id == '#')
		{
			for (int j = blocks[i].second[blocks[i].second.size() - 1].first; j < data[blocks[i].second[0].second].size()-1; j++)
			{
				if (data[blocks[i].second[0].second][j+1] != '-')
				{
					is_free = false;
					return is_free;
				}
			}
		}
	}
	return is_free;
}

bool Gameboard::QueryDuplicateMove()
{
	bool is_duplicate = false;
	std::string gamestate = "";

	for (int c = 0; c < data.size(); c++) {
		for (int r = 0; r < data[0].size(); r++)
		{
			gamestate += data[c][r];
		}
	}

	if (gamestate_history.count(gamestate) != 0)
		is_duplicate = true;

	return is_duplicate;
}

void Gameboard::PrintBoard()
{
	for (int c = 0; c < data.size(); c++) {
		for (int r = 0; r < data[0].size(); r++)
		{
			std::cout << data[c][r] << " ";
		}
		std::cout << std::endl;
	}
}

void Gameboard::PrintSolution(std::pair<std::string, std::string> _solved_state)
{
	std::vector<std::string> _solution_states = { _solved_state.second };
	std::pair<int, std::string> _prev_state = gamestate_history[_solved_state.first];
	while (_prev_state.second != "First")
	{
		_prev_state = gamestate_history[_prev_state.second];
		_solution_states.push_back(_prev_state.second);
	}
	int _step_counter = 0;
	for (int i = _solution_states.size() - 2; i >= 0; i--)
	{
		std::system("pause");
		std::system("cls");
		GoToState(_solution_states[i]);
		std::cout << "===== " << _step_counter << " =====" << std::endl;
		PrintBoard();
		_step_counter++;
	}
	
	std::system("pause");
	std::system("cls");
	GoToState(_solved_state.first);
	std::cout << "===== " << _step_counter << " =====" << std::endl;
	PrintBoard();
}

Block::Block(char _id)
{
	id = _id;
	is_horizontal = false;
}

void Block::Calculate(std::vector<std::pair<int, int>> _coords) 
{
	if (_coords[0].second == _coords[1].second)
		is_horizontal = true;
}

std::pair<char, std::vector<int>> Block::GetMoves(std::vector<std::pair<int, int>> _coords, std::vector<std::vector<char>> _data)
{
	std::pair<char, std::vector<int>> _moves(id, {});

	int _back_search;
	int _front_search;

	if (is_horizontal)
	{
		_back_search = _coords[0].first - 1;
		_front_search = _coords[_coords.size() - 1].first + 1;
		
		while (_back_search > -1 && _data[_coords[0].second][_back_search] == '-')
		{
			_moves.second.push_back(_back_search - _coords[0].first);
			_back_search--;
		}

		while (_front_search < _data[0].size() && _data[_coords[0].second][_front_search] == '-')
		{
			_moves.second.push_back(_front_search - _coords[_coords.size() - 1].first);
			_front_search++;
		}
	}
	else
	{
		_back_search = _coords[0].second - 1;
		_front_search = _coords[_coords.size() - 1].second + 1;

		while (_back_search > -1 && _data[_back_search][_coords[0].first] == '-')
		{
			_moves.second.push_back(_back_search - _coords[0].second);
			_back_search--;
		}

		while (_front_search < _data[0].size() && _data[_front_search][_coords[0].first] == '-')
		{
			_moves.second.push_back(_front_search - _coords[_coords.size() - 1].second);
			_front_search++;
		}
	}

	return _moves;
}