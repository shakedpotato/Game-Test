#pragma once
#include "CommonWinApp.h"
#include "OrderedSingleton.h"

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
// 
// �N���X: GameWindow 
// �T�v  : �Q�[�����C���E�B���h�E�̐���
// ���ӓ_: GameSystemObject�e���v���[�g����C���X�^���X���擾���邱�ƁB
// 
// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

class GameWindow final
{
private:
	friend class OrderedSingleton<GameWindow>;
	GameWindow();
public:
	~GameWindow();
public:
	void Open( unsigned int width, unsigned int height,
			   HINSTANCE hInstance, const TCHAR* className, const TCHAR* caption, DWORD  windowStyle, int cmdShow );

private:
	unsigned int m_Width;
	unsigned int m_Height;
	HWND m_hWnd;
	tstring* m_pClassName;
	tstring* m_pCaption;
	DWORD m_WindowStyle;
	int m_CmdShow;

public:
	HWND HWND( void ) { return m_hWnd; }
	unsigned int Width( void ) { return m_Width; }
	unsigned int Height( void ) { return m_Height; }
	void SetCaption( const TCHAR* caption );

};

