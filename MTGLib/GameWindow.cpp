#include "main.h"

#include "GameWindow.h"

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
//
// �N���X: GameWindow
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
		if( MessageBox( hWnd, "�{���ɏI�����Ă�낵���ł���", "�m�F", MB_OKCANCEL | MB_DEFBUTTON2 ) == IDOK )
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
/* �T�v  : �E�B���h�E�̐���                                                            */
/* ----------------------------------------------------------------------------------- */
void GameWindow::Open( unsigned int width, unsigned int height, 
					   HINSTANCE hInstance, 
					   const TCHAR* className, 
					   const TCHAR* caption,
					   DWORD  windowStyle,
					   int cmdShow )
{
	/* �G���[���� */
	assert( className );
	assert( caption );

	/* �����o ������ */
	m_pClassName = new tstring( className );
	m_pCaption = new tstring( caption );
	m_WindowStyle = windowStyle;
	m_CmdShow = cmdShow;
	m_Width = width;
	m_Height = height;

	/* WNDCLASS ������ */
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = m_pClassName->c_str();
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = ( HBRUSH ) ( COLOR_BACKGROUND + 1 );

	RegisterClass( &wc ); // �V�X�e���ւ̓o�^

	/* �E�B���h�E�����ʒu�ƃT�C�Y�̐ݒ� */
	RECT WindowRect = { 0, 0, ( LONG )m_Width, ( LONG )m_Height }; // �E�B���h�E�̃N���C�A���g�̈�����߂�

	/*  AdjustWIndowRect */
	/* �T�v: �ݒ肵�����N���C�A���g�̈����������RECT�\���̂̈�����n���A�t���[�����݂̃E�B���h�E�̗̈�����߁A�ϐ��ɓn��              */
	/* �ߒl: BOOL                                                                                                                      */
	/* https://msdn.microsoft.com/ja-jp/library/cc364870.aspx �ڂ����͂��� */
	AdjustWindowRect( &WindowRect,   // LPRECT lpRect  �E�B���h�E�̋�`�̈� ( �ݒ肵�����N���C�A���g�̈���������ă|�C���^��n���A�t���[�����݂̗̈���������� )
					  m_WindowStyle, // DWORD  dwStyle ��������E�B���h�E�̃X�^�C��
					  FALSE );       // BOOL bMenu �E�B���h�E�Ƀ��j���[�o�[���������邩�ǂ���

	const long lWindowWidth = WindowRect.right - WindowRect.left;
	const long lWindowHeight = WindowRect.bottom - WindowRect.top;


	// �E�B���h�E�����ʒu�����j�^�[�����ɐݒ肷��
	const long lMonitorWidth = ( long ) GetSystemMetrics( SM_CXSCREEN ); // int nIndex �V�X�e�����g���b�N�܂��͌��݂̍\��
																	   // GetSystemMetrics 
																	   // �T�v: ���܂��܂ȃV�X�e�����g���b�N�̒l�i�\���v�f�̕��ƍ����j�ƃV�X�e���̌��݂̍\�����擾����B
																	   // �ߒl: int �e�퐔��   
																	   // https://msdn.microsoft.com/ja-jp/library/cc429812.aspx �ڂ����͂��� 
	const long lMonitorHeight = ( long ) GetSystemMetrics( SM_CYSCREEN );

	const long lWindowBornX = max( ( ( lMonitorWidth - lWindowWidth ) / 2 ), 0 ); // max( a, b ) windows.h
	const long lWindowBornY = max( ( ( lMonitorHeight - lWindowHeight ) / 2 ), 0 );

	/* �E�B���h�E�̐��� */
	m_hWnd = CreateWindow(
		m_pClassName->c_str(),
		m_pCaption->c_str(),
		m_WindowStyle,
		lWindowBornX, // �����ʒu X
		lWindowBornY, //          Y
		lWindowWidth, // �E�B���h�E��Width ( �N���C�A���g�̈� �{ �t���[�� )
		lWindowHeight,  // �E�B���h�EHeight ( �N���C�A���g�̈� �{ �t���[�� )
		NULL,
		NULL,
		hInstance,
		NULL );

	assert( m_hWnd ); // �E�B���h�E�n���h����NULL�łȂ����Ƃ��m�F����

	ShowWindow( m_hWnd, m_CmdShow );
	UpdateWindow( m_hWnd );			
}

/* ----- GameWindow::SetCaption ------------------------------------------------------ */
/* �T�v  : �L���v�V�����̕ύX                                                          */
/* ----------------------------------------------------------------------------------- */
void GameWindow::SetCaption( const TCHAR* caption )
{
	SetWindowText( m_hWnd, caption );
}