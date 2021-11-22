#pragma once

#include "Common.h"
#include "Singleton.h"
#include "InputHandler.h"

using namespace std;

class GameController: public Singleton<GameController>
{
	enum class AiDepth : int32_t
	{
		EASY = 2,
		MEDIUM = 4,
		HARD = 6,
	};

public:

	GameController(token);
	void Run();

private:

	void StartGame();
	void StartHandleInput();
	void InitBoard(int size);
	void InitSimbols();
	void InitAiLevelMap();
	bool IsValidMove(const Move& move) const;
	void PrintBoard();
	void clearConsole();
	void ChooseSide();

private:

	vector<vector<int>> _gameBoard;
	map<string, AiDepth> _aiLevelMap;
	map<int, char> _symbols;
	char _playerSymbol;
	char _AiSymbol;
	int _aiLevel;

};

