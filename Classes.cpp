#include "Classes.h"

Gameboard::Gameboard(std::vector<std::vector<char>> a_data)
{
	data = a_data;
	UpdateBlocks();
}

void Gameboard::UpdateData()
{
	for (int c = 0; c < data.size(); c++)
	{
		std::fill(data[c].begin(), data[c].end(), '-');
	}

	for (auto block = blocks.begin(); block != blocks.end(); block++)
	{
		for (int coord_ind = 0; coord_ind < block->second.coords.size(); coord_ind++)
		{
			data[block->second.coords[coord_ind].first][block->second.coords[coord_ind].second] = block->first;
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

			if (blocks.count(data[c][r]) == 0)
			{
				Block NewBlock(data[c][r]);
				blocks[data[c][r]] = NewBlock;
			}

			blocks[data[c][r]].coords.push_back({ c, r });
		}
	}
	for (auto block = blocks.begin(); block != blocks.end(); block++)
		block->second.Calculate();
}

std::vector<std::pair<char, std::vector<int>>> Gameboard::GetAllMoves()
{
	std::vector<std::pair<char, std::vector<int>>> _movelist;

	for (auto block = blocks.begin(); block != blocks.end(); block++)
	{
		_movelist.push_back({ block->first, block->second.GetMoves(data) });
	}

	return _movelist;
}

void Gameboard::MoveBlock(std::pair<char, int> a_move)
{
	Block& block = blocks[a_move.first];

	if (block.is_horizontal)
	{
		for (int coord = 0; coord < block.coords.size(); coord++)
		{
			block.coords[coord] = { block.coords[coord].first, block.coords[coord].second + a_move.second };
		}
	}

	else
	{
		for (int coord = 0; coord < block.coords.size(); coord++)
		{
			block.coords[coord] = { block.coords[coord].first + a_move.second, block.coords[coord].second };
		}
	}

	UpdateData();
}

std::pair<std::string, std::string> Gameboard::SaveState(std::string a_parent_state)
{
	std::string gamestate = "";

	for (int c = 0; c < data.size(); c++) {
		for (int r = 0; r < data[0].size(); r++)
		{
			gamestate += data[c][r];
		}
	}
	gamestate_history[gamestate] = { movecount, a_parent_state };

	return { gamestate, a_parent_state };
}

void Gameboard::GoToState(std::string a_state)
{
	int char_ind = 0;
	for (int c = 0; c < data.size(); c++) {
		for (int r = 0; r < data[0].size(); r++)
		{
			data[c][r] = a_state[char_ind];
			char_ind++;
		}
	}
	UpdateBlocks();
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

bool Gameboard::QuerySolved()
{
	bool is_free = true;

	Block& block = blocks['#'];

	for (int coord = block.coords[block.coords.size() - 1].second + 1; coord < data[block.coords[0].second].size(); coord++)
	{
		if (data[block.coords[0].first][coord] != '-')
		{
			is_free = false;
			return is_free;
		}
	}
	return is_free;
}

void Gameboard::Solve(bool a_debug)
{
	auto start_time = std::chrono::high_resolution_clock::now();

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
				std::vector<std::pair<char, std::vector<int>>> movelist = GetAllMoves();

				for (int block = 0; block < movelist.size(); block++)
				{
					for (int block_move = 0; block_move < movelist[block].second.size(); block_move++)
					{
						MoveBlock(std::pair<char, int> { movelist[block].first, movelist[block].second[block_move] });
						if (QueryDuplicateMove() == false)
						{
							std::pair<std::string, std::string> current_state = SaveState(prev_state.second);

							if (a_debug == true)
							{
								std::cout << " -------- " << std::endl;
								PrintBoard();
							}

							new_gamestates++;


							if (QuerySolved())
							{
								auto finish_time = std::chrono::high_resolution_clock::now();
								std::chrono::duration<double> time_elapsed = finish_time - start_time;

								std::cout << "======== Solution ========" << std::endl;
								std::cout << "Time Elapsed: " << time_elapsed.count() << std::endl;
								std::cout << "Moves: " << (gamestate_history[current_state.first].first) + 1 << std::endl;
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

void Gameboard::PrintSolution(std::pair<std::string, std::string> a_solved_state)
{
	std::vector<std::string> solution_states = { a_solved_state.second };
	std::pair<int, std::string> prev_state = gamestate_history[a_solved_state.first];
	while (prev_state.second != "First")
	{
		prev_state = gamestate_history[prev_state.second];
		solution_states.push_back(prev_state.second);
	}
	int step_counter = 0;
	for (int i = solution_states.size() - 2; i >= 0; i--)
	{
		std::system("pause");
		std::system("cls");
		GoToState(solution_states[i]);
		std::cout << "==== " << step_counter << " ====" << std::endl;
		PrintBoard();
		step_counter++;
	}

	std::system("pause");
	std::system("cls");
	GoToState(a_solved_state.first);
	std::cout << "==== " << step_counter << " ====" << std::endl;
	PrintBoard();
	step_counter++;

	std::system("pause");
	std::system("cls");
	blocks.erase('#');
	UpdateData();
	std::cout << "==== " << step_counter << " ====" << std::endl;
	PrintBoard();
}

Block::Block()
{
	id = 'X';
	is_horizontal = false;
}

Block::Block(char a_id)
{
	id = a_id;
	is_horizontal = false;
}


void Block::Calculate()
{
	if (coords[0].first == coords[1].first)
		is_horizontal = true;
}

std::vector<int> Block::GetMoves(std::vector<std::vector<char>> a_data)
{
	std::vector<int> moves;

	int back_search;
	int front_search;

	if (is_horizontal)
	{
		back_search = coords[0].second - 1;
		front_search = coords[coords.size() - 1].second + 1;

		while (back_search > -1 && a_data[coords[0].first][back_search] == '-')
		{
			moves.push_back(back_search - coords[0].second);
			back_search--;
		}

		while (front_search < a_data[0].size() && a_data[coords[0].first][front_search] == '-')
		{
			moves.push_back(front_search - coords[coords.size() - 1].second);
			front_search++;
		}
	}
	else
	{
		back_search = coords[0].first - 1;
		front_search = coords[coords.size() - 1].first + 1;

		while (back_search > -1 && a_data[back_search][coords[0].second] == '-')
		{
			moves.push_back(back_search - coords[0].first);
			back_search--;
		}

		while (front_search < a_data[0].size() && a_data[front_search][coords[0].second] == '-')
		{
			moves.push_back(front_search - coords[coords.size() - 1].first);
			front_search++;
		}
	}

	return moves;
}