#pragma once
#include <Windows.h>
#include <iostream>
#include <map>
class InputManager
{
public:
	bool GetButton(int key);
	bool GetButtonDown(int key);
private:
	std::map<int, bool> mapPressedInput;
	bool isP = false;
};

