#pragma once

template <typename T>
class Singleton
{

public:

	static T& Instance()
	{
		static T instance{ token{} };
		return instance;
	}

protected:
	class token {};
	Singleton() {};

};

