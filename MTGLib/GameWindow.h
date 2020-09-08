#pragma once
#include "CommonWinApp.h"
#include "OrderedSingleton.h"

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
// 
// クラス: GameWindow 
// 概要  : ゲームメインウィンドウの生成
// 注意点: GameSystemObjectテンプレートからインスタンスを取得すること。
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

