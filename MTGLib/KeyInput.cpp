#include "KeyInput.h"

KeyInput::KeyInput()
{
	memset(m_OldKeyState, 0, 256);
	memset(m_KeyState, 0, 256);
}

KeyInput::~KeyInput()
{
	// null
}

void KeyInput::Update()
{
	memcpy(m_OldKeyState, m_KeyState, 256);
	bool b = GetKeyboardState(m_KeyState);
}

bool KeyInput::GetKeyPress(BYTE keyCode) const
{
	return (m_KeyState[keyCode] & 0x80);
}

bool KeyInput::GetKeyTrigger(BYTE keyCode) const
{
	return (m_KeyState[keyCode] & 0x80) & !(m_OldKeyState[keyCode] & 0x80);
}

bool KeyInput::GetKeyRelease(BYTE keyCode) const
{
	return (!(m_KeyState[keyCode] & 0x80)) & (m_OldKeyState[keyCode] & 0x80);
}