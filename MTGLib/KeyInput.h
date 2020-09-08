#pragma once
#include "OrderedSingleton.h"

class KeyInput
{
private:
	friend class OrderedSingleton<KeyInput>;
	KeyInput();
	~KeyInput();
public:
	void Update();
private:
	BYTE m_OldKeyState[ 256 ];
	BYTE m_KeyState[ 256 ];
public:
	bool GetKeyPress(BYTE keyCode) const;
	bool GetKeyTrigger(BYTE keyCode) const;
	bool GetKeyRelease(BYTE keyCode) const;
};

