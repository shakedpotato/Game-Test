#include "main.h"

#include "GameWindow.h"

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
//
// クラス: GameWindow
//
// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-


GameWindow::GameWindow()
	:
	m_hWnd( nullptr ),
	m_pClassName( nullptr ),
	m_pCaption( nullptr ),
	m_WindowStyle(),
	m_Width( 0 ),
	m_Height( 0 ),
	m_CmdShow( 0 )

{

}

GameWindow::~GameWindow()
{
	delete m_pCaption;
	delete m_pClassName;
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
		{
			SendMessage( hWnd, WM_CLOSE, 0, 0 );
		}
		break;
	case WM_CLOSE:
		if( MessageBox( hWnd, "本当に終了してよろしいですか", "確認", MB_OKCANCEL | MB_DEFBUTTON2 ) == IDOK )
		{
			DestroyWindow( hWnd );
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;
	};

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

/* ----- GameWindow::Open ------------------------------------------------------------ */
/* 概要  : ウィンドウの生成                                                            */
/* ----------------------------------------------------------------------------------- */
void GameWindow::Open( unsigned int width, unsigned int height, 
					   HINSTANCE hInstance, 
					   const TCHAR* className, 
					   const TCHAR* caption,
					   DWORD  windowStyle,
					   int cmdShow )
{
	/* エラー処理 */
	assert( className );
	assert( caption );

	/* メンバ 初期化 */
	m_pClassName = new tstring( className );
	m_pCaption = new tstring( caption );
	m_WindowStyle = windowStyle;
	m_CmdShow = cmdShow;
	m_Width = width;
	m_Height = height;

	/* WNDCLASS 初期化 */
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = m_pClassName->c_str();
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = ( HBRUSH ) ( COLOR_BACKGROUND + 1 );

	RegisterClass( &wc ); // システムへの登録

	/* ウィンドウ生成位置とサイズの設定 */
	RECT WindowRect = { 0, 0, ( LONG )m_Width, ( LONG )m_Height }; // ウィンドウのクライアント領域を求める

	/*  AdjustWIndowRect */
	/* 概要: 設定したいクライアント領域を持たせたRECT構造体の引数を渡し、フレーム込みのウィンドウの領域を求め、変数に渡す              */
	/* 戻値: BOOL                                                                                                                      */
	/* https://msdn.microsoft.com/ja-jp/library/cc364870.aspx 詳しくはここ */
	AdjustWindowRect( &WindowRect,   // LPRECT lpRect  ウィンドウの矩形領域 ( 設定したいクライアント領域を持たせてポインタを渡し、フレーム込みの領域を持たせる )
					  m_WindowStyle, // DWORD  dwStyle 生成するウィンドウのスタイル
					  FALSE );       // BOOL bMenu ウィンドウにメニューバーを持たせるかどうか

	const long lWindowWidth = WindowRect.right - WindowRect.left;
	const long lWindowHeight = WindowRect.bottom - WindowRect.top;


	// ウィンドウ生成位置をモニター中央に設定する
	const long lMonitorWidth = ( long ) GetSystemMetrics( SM_CXSCREEN ); // int nIndex システムメトリックまたは現在の構成
																	   // GetSystemMetrics 
																	   // 概要: さまざまなシステムメトリックの値（表示要素の幅と高さ）とシステムの現在の構成を取得する。
																	   // 戻値: int 各種数字   
																	   // https://msdn.microsoft.com/ja-jp/library/cc429812.aspx 詳しくはここ 
	const long lMonitorHeight = ( long ) GetSystemMetrics( SM_CYSCREEN );

	const long lWindowBornX = max( ( ( lMonitorWidth - lWindowWidth ) / 2 ), 0 ); // max( a, b ) windows.h
	const long lWindowBornY = max( ( ( lMonitorHeight - lWindowHeight ) / 2 ), 0 );

	/* ウィンドウの生成 */
	m_hWnd = CreateWindow(
		m_pClassName->c_str(),
		m_pCaption->c_str(),
		m_WindowStyle,
		lWindowBornX, // 生成位置 X
		lWindowBornY, //          Y
		lWindowWidth, // ウィンドウのWidth ( クライアント領域 ＋ フレーム )
		lWindowHeight,  // ウィンドウHeight ( クライアント領域 ＋ フレーム )
		NULL,
		NULL,
		hInstance,
		NULL );

	assert( m_hWnd ); // ウィンドウハンドルがNULLでないことを確認する

	ShowWindow( m_hWnd, m_CmdShow );
	UpdateWindow( m_hWnd );			
}

/* ----- GameWindow::SetCaption ------------------------------------------------------ */
/* 概要  : キャプションの変更                                                          */
/* ----------------------------------------------------------------------------------- */
void GameWindow::SetCaption( const TCHAR* caption )
{
	SetWindowText( m_hWnd, caption );
}