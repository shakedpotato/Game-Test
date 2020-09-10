// ===============================================================================================                                        
// 概要:                                                                             
//                                                                                              
// 作成日 XX/XX/XXXX Author: Hashimoto Yuto                                       
// ===============================================================================================

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
// 
// インクルードファイル
// 
// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
#include "MyGame.h"
#include "TimePiece.h"
#include "Timer.h"
#include "DeltaTimerUpdater.h"
#include "GameWindow.h"
#include "KeyInput.h"
#include "Texture.h"
#include "SceneManager.h"
#include "RenderingExecuter.h"

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
// 
// マクロ定義
// 
// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
#define DEFAULT_CLASS_NAME         _T( "GameWindow" )
#define DEFAULT_WINDOW_CAPTION     _T( "MyGame" )
#define DEFAULT_WINDOW_STYLE       ( WS_OVERLAPPEDWINDOW ^ ( WS_MAXIMIZEBOX | WS_THICKFRAME ) )
#define DEFAULT_TIMER_ID           1
#define DEFAULT_TARGET_FPS         60.0
#define DEFAULT_WINDOW_WIDTH       1024
#define DEFAULT_WINDOW_HEIGHT      756

using GBSFinalizer = OrderedSingletonFinalizer;
using DTimer = OrderedSingleton<DeltaTimer>;
using KeyInputObj = OrderedSingleton<KeyInput>;
// using RendererObj = GameBasicSystemObject < Renderer>;
 // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
 //
 // クラス: MyGame
 //
 // -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

MyGame* MyGame::m_pInstance = nullptr;

MyGame::MyGame()
	:
	m_TimerId( 0 ),
	m_TimeCount( 0.0 )
{
	
}

MyGame* MyGame::Instance( void )
{
	if( m_pInstance == nullptr )
	{
		m_pInstance = new MyGame();
	}

	return m_pInstance;
}

int MyGame::Main()
{
	MSG msg = {};
	
	MyGame::Init();

	/* --- メッセージループ --- */ // 
	do
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			OrderedSingleton<TimePiece>::Instance()->Update();
			
			double dt = OrderedSingleton<TimePiece>::Instance()->GetDeltaTime();

			m_TimeCount += dt;

			if( m_TimeCount < ( 1.0 / DEFAULT_TARGET_FPS ) )
			{
				// null
			}
			else
			{
				m_TimeCount = 0.0;
				// ゲーム処理
				MyGame::Update();
				MyGame::Render();
			}
		}
	} while( msg.message != WM_QUIT );

	// ゲームの終了処理
	Finalize();
	return ( int ) msg.wParam;

}

void MyGame::Init()
{
	OrderedSingleton<GameWindow>::Instance()->Open( DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT,
													   GetHInstance(), 
													   DEFAULT_CLASS_NAME, 
													   DEFAULT_WINDOW_CAPTION,
													   DEFAULT_WINDOW_STYLE, 
													   GetCmdShow() );
	
	OrderedSingleton<TimePiece>::Instance()->Start();
	OrderedSingleton<KeyInput>::Instance();
	OrderedSingleton<TextureManager>::Instance();
	OrderedSingleton<RenderingExecuter>::Instance();
	
	OrderedSingleton<SceneManager>::Instance()->Init();
}

void MyGame::Update()
{
	OrderedSingleton<DeltaTimerUpdater>::Instance()->UpdateTimer();
	SetFpsOnCaption();
	OrderedSingleton<KeyInput>::Instance()->Update();
	OrderedSingleton<SceneManager>::Instance()->UpdateScenes();
}

void MyGame::Render()
{
	// GameBasicSystemObject<Manager>::GetInstance()->Draw();
	OrderedSingleton<RenderingExecuter>::Instance()->RenderAll();
}

void MyGame::Finalize()
{
	OrderedSingletonFinalizer::FinalizeAll();
}

void MyGame::SetFpsOnCaption( void )
{


	 double fps = 1.0 / DTimer::Instance()->deltaTimed();
	 char text[ 256 ];
	 sprintf_s( text, 256, "fps = %.3lf", fps );
	 
	 OrderedSingleton<GameWindow>::Instance()->SetCaption( text );
}
