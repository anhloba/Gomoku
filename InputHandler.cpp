#include "InputHandler.h"

void InputHandler::BordSizeInput(int& boardSize)
{
	char answer;

	cout << "Do you want to play at the default size of board? y/n : ";
	cin >> answer;
	if (answer == 'N' || answer == 'n')
	{
	enterSize:
		cout << "Please enter size from eight to fifteen: ";
		cin >> answer;

		boardSize = atoi(&answer);
		if (boardSize < 8 || boardSize > 15)
		{
			cout << "Please enter correct size!" << endl;
			goto enterSize;
		}
	}
}

bool InputHandler::IsRestartInput()
{
	char answer;

replay:
	cout << "Do you want to play another game? y/n : ";
	cin >> answer;
	if (answer == 'N' || answer == 'n')
	{
		return false;
	}
	else if (answer == 'Y' || answer == 'y')
	{
		return true;
	}
	else
	{
		cout << "I don't understand you." << endl;
		goto replay;
	}
}

void InputHandler::PlayerMoveInput(Move& move, const int & size)
{
	char answer1;
	char answer2;
	cout << "Your turn." << endl;
	
enterRow:
	cout << "Enter number of ROW of your move: ";
	cin >> answer1;
	move.i = atoi(&answer1) - 1;
	if (move.i < 0 || move.i >= size)
	{
		cout << "Please enter correct number!" << endl;
		goto enterRow;
	}

enterColumn:
	cout << "Enter number of COLUMN of your move: ";
	cin >> answer2;
	move.j = atoi(&answer2) - 1;
	if (move.j < 0 || move.j >= size)
	{
		cout << "Please enter correct number!" << endl;
		goto enterColumn;
	}
}

string InputHandler::AiLevelInput()
{
	string answer;

enterLevel:
	cout << "Choos difficulty level of AI - Easy(E)/Medium(M)/Hard(H): ";
	cin >> answer;
	if (answer != "e" && answer != "E" && answer != "m" && answer != "M" && answer != "h" && answer != "H")
	{
		cout << "Please enter correct level!" << endl;
		goto enterLevel;
	}

	return answer;
}
