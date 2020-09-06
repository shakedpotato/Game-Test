#include "main.h"
#include "MyGame.h"


static HINSTANCE g_hInstnace = NULL;

HINSTANCE GetHInstance( void )
{
	return g_hInstnace;
}

static int g_CmdShow = NULL;

int GetCmdShow( void )
{
	return g_CmdShow;
}

int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nShowCmd )
{
	g_hInstnace = hInstance;
	g_CmdShow = nShowCmd;
	
	MyGame* pGame = MyGame::Instance();
	int result = pGame->Main();

	delete pGame;
	return result;
}