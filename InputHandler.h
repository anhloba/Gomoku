#pragma once

#include "Common.h"
#include "Singleton.h"

using namespace std;

class InputHandler : public Singleton<InputHandler>
{
public:

	InputHandler(token) {}

	void BordSizeInput(int& boardSize);
	bool IsRestartInput();
	void PlayerMoveInput(Move& move, const int& size);
	string AiLevelInput();
};

