#include "InputManager.h"

bool InputManager::GetButton(int key)
{
	return GetKeyState(key) & 0x8000;
}

bool InputManager::GetButtonDown(int key)
{
	if (this->mapPressedInput.find(key) == this->mapPressedInput.end())
	{
		this->mapPressedInput[key] = false;
	}
	if (GetKeyState(key) & 0x8000)
	{
		if (!this->mapPressedInput[key])
		{
			this->mapPressedInput[key] = true;
			return true;
		}
	}
	else
	{
		this->mapPressedInput[key] = false;
	}
	return false;
}