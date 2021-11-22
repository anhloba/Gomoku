#include <Windows.h>
#include "GameController.h"
#include "AI.h"

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

GameController::GameController(token)
{
	InitAiLevelMap();
}

void GameController::StartGame()
{

	StartHandleInput();

	mt19937 mt_rand(time(0));

	int size = _gameBoard.size();

	AI::Bot killer(size, size);

	killer.SetAiDepth(_aiLevel);

	Move AiMove{ mt_rand() % size, mt_rand() % size, 0 };
	Move playerMove;

	char answer;

	bool AiTurn = _playerSymbol == 'O';
	bool flag = true;

	PrintBoard();

	std::chrono::steady_clock::time_point begin;
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	while (true) {

		begin = std::chrono::steady_clock::now();
		if (AiTurn)
		{
			long long tmp = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
			if (tmp < 3000)
			{
				while (tmp < 3000)
				{
					tmp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count();
				}
			}

			begin = std::chrono::steady_clock::now();

			AiTurn = false;
			_gameBoard[AiMove.i][AiMove.j] = 1;
			clearConsole();
			PrintBoard();
			cout << "My move is : Row " << AiMove.i + 1 << ", Column " << AiMove.j + 1 << " !" << endl;
			if (killer.CheckWin(_gameBoard, AiMove.i, AiMove.j))
			{
				cout << "I am winer!)" << endl;
				break;
			}
		}

		if (!AiTurn)
		{
			AiTurn = true;
			cout << "Your turn." << endl;

		enterPos:
			InputHandler::Instance().PlayerMoveInput(playerMove, size);

			if (IsValidMove(playerMove))
			{
				_gameBoard[playerMove.i][playerMove.j] = -1;
				clearConsole();
				PrintBoard();
				cout << "Your move is : Row " << playerMove.i + 1 << ", Column " << playerMove.j + 1 << " !" << endl;
			}
			else
			{
				cout << "Point is already taken. Try again!" << endl;
				goto enterPos;
			}

			if (killer.CheckWin(_gameBoard, playerMove.i, playerMove.j))
			{
				cout << "CONGRATULATIONS! You are winer!)" << endl;
				break;
			}
	
		}

		killer.SetGameBoard(_gameBoard);
		AiMove = killer.FindBestMove();

		end = std::chrono::steady_clock::now();
	}
}

void GameController::Run()
{

playAgain:
	SetConsoleTextAttribute(console, 15);

	StartGame();

	SetConsoleTextAttribute(console, 15);

	if (InputHandler::Instance().IsRestartInput())
	{
		clearConsole();
		goto playAgain;
	}
}

void GameController::ChooseSide()
{
	mt19937 mt_rand(time(0));

	int side = mt_rand() % 2;

	if (side)
	{
		_playerSymbol = 'O';
		_AiSymbol = 'X';
		cout << "You play as 'O'" << endl;
		cout << "My move first)" << endl;
	}
	else
	{
		_playerSymbol = 'X';
		_AiSymbol = 'O';
		cout << "You play as 'X'" << endl;
		cout << "Your move first)" << endl;
	}
}

void GameController::StartHandleInput()
{
	int boardSize = 8;

	InputHandler::Instance().BordSizeInput(boardSize);

	InitBoard(boardSize);

	string aiLevel = InputHandler::Instance().AiLevelInput();
	_aiLevel = static_cast<int>(_aiLevelMap[aiLevel]);

	ChooseSide();

	InitSimbols();

	cout << "Let's GO!)" << endl;
}

void GameController::InitBoard(int size)
{
	vector <vector<int>> board(size, vector<int>(size));
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			board[i][j] = 0;
		}
	}
	_gameBoard = board;
}

void GameController::InitSimbols()
{
	_symbols[0] = '.';
	_symbols[-1] = _playerSymbol;
	_symbols[1] = _AiSymbol;
}

void GameController::InitAiLevelMap()
{
	_aiLevelMap["e"] = AiDepth::EASY;
	_aiLevelMap["E"] = AiDepth::EASY;
	_aiLevelMap["m"] = AiDepth::MEDIUM;
	_aiLevelMap["M"] = AiDepth::MEDIUM;
	_aiLevelMap["h"] = AiDepth::HARD;
	_aiLevelMap["H"] = AiDepth::HARD;
}

bool GameController::IsValidMove(const Move& move) const
{
	return _gameBoard[move.i][move.j] == 0;
}

void GameController::PrintBoard()
{
	int size = _gameBoard.size();
	cout << " ";
	for (int n = 1; n <= size; n++)
	{
		cout << "\t" << n;
	}
	cout << endl << endl;

	for (int i = 0; i < size; i++)
	{
		SetConsoleTextAttribute(console, 15);
		cout << i + 1;
		for (int j = 0; j < size; j++)
		{
			int key = _gameBoard[i][j];
			if (_symbols[key] == _playerSymbol)
			{
				SetConsoleTextAttribute(console, 2);
			}
			else if (_symbols[key] == _AiSymbol)
			{
				SetConsoleTextAttribute(console, 4);
			}
			else
			{
				SetConsoleTextAttribute(console, 15);
			}
			cout << "\t" << _symbols[key];
		}
		cout << endl << endl << endl;
	}
}

void GameController::clearConsole() {
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	SetConsoleCursorPosition(console, topLeft);
}

