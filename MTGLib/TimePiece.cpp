// ===============================================================================================
// �v���W�F�N�g��: MyGame
// �t�@�C����: TikTimer.cpp
// �T�v: Windows�̍�����\��p���āA�A�v���P�[�V�����̎��Ԃ̌o�ߎ��Ԃ��v������B
// �쐬�� ??/??/???? Author: Hashimoto Yuto AT12E-275-29                                    
// ===============================================================================================

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
// 
// �C���N���[�h
// 
// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
#include "CommonWinApp.h"
#include "TimePiece.h"

//
// �R���X�g���N�^
//
TimePiece::TimePiece()
{
	this->isTimerStopped  = true; 
	this->InitTickPerSec = 0;
	this->StopTime        = 0;    
	this->LastElapsedTime = 0;    
	this->BaseTime        = 0;    
	this->DeltaTime       = 0.0f;

	// ������\�p�t�H�[�}���X�J�E���^���g���̎擾
	LARGE_INTEGER tmpTicksPerSec = { 0 };
	QueryPerformanceFrequency( &tmpTicksPerSec );
	this->InitTickPerSec = tmpTicksPerSec.QuadPart;

}

/* ----- TimePiece::Reset ------------------------------------------------------------ */
/* �T�v  :�^�C�}�[�̃��Z�b�g                                                           */
/* �߂�l: �Ȃ�                                                                        */
/* ----------------------------------------------------------------------------------- */
void TimePiece::Reset( void )
{
	LARGE_INTEGER Time = GetAdjustedCurrentTime();
	this->BaseTime = this->LastElapsedTime = Time.QuadPart;
	this->StopTime = 0;
	this->isTimerStopped = true;

	LARGE_INTEGER currentFrequency;
	QueryPerformanceFrequency( &currentFrequency );
	this->InitTickPerSec = currentFrequency.QuadPart;
}

/* ----- TimePiece::Start ------------------------------------------------------------ */
/* �T�v  :�^�C�}�[�̏������ƌv���J�n�B                                                 */
/*        �{�N���X�̗��p�O�Ɉ�x�����K���ĂԂ��ƁB                                     */
/* �߂�l: �Ȃ�                                                                        */
/* ----------------------------------------------------------------------------------- */
void TimePiece::Start( void )
{
	// ���ݎ������擾
	LARGE_INTEGER Time = { 0 };
	QueryPerformanceCounter( &Time );

	this->StopTime = 0;
	this->BaseTime = this->LastElapsedTime = Time.QuadPart;
	this->isTimerStopped = false;
}

/* ----- TimePiece::Stop ------------------------------------------------------------- */
/* �T�v  : �^�C�}�[���~����B                                                        */
/* �߂�l: �Ȃ�                                                                        */
/* ----------------------------------------------------------------------------------- */
void TimePiece::Stop( void )
{
	if( this->isTimerStopped ) { return; }

	LARGE_INTEGER Time = { 0 };
	QueryPerformanceCounter( &Time );
	// ��~���Ԃ��L�^���Ē�~�t���O�𗧂Ă�
	this->LastElapsedTime = this->StopTime = Time.QuadPart;
	this->isTimerStopped = true;
}

/* ----- TimePiece::Put1msAhead ------------------------------------------------------ */
/* �T�v  : ��~���Ԃ�1ms���̎��g�������₷�B                                           */
/* �߂�l: �Ȃ�                                                                        */
/* ----------------------------------------------------------------------------------- */
void TimePiece::Put1msAhead( void )
{
	this->StopTime += this->InitTickPerSec / 10;
}

/* ----- TimePiece::GetTime ---------------------------------------------------------- */
/* �T�v  : Start()���Ă΂�Ă���̌o�ߎ��Ԃ�߂��B                                     */
/* �߂�l: ( double ) Start()�Ăяo������̌o�ߎ���                                    */
/* ----------------------------------------------------------------------------------- */
double TimePiece::GetTime( void )
{
	LARGE_INTEGER Time = GetAdjustedCurrentTime();
	
	return (double) ( Time.QuadPart - this->BaseTime ) / ( double ) this->InitTickPerSec;
}

/* ----- TimePiece::GetAbsoluteTime -------------------------------------------------- */
/* �T�v  : ���݂̎��Ԃ��擾(�H)                                                        */
/* �߂�l: ( double )���݂̎���                                                        */
/* ----------------------------------------------------------------------------------- */
double TimePiece::GetAbsoluteTime( void )
{
	LARGE_INTEGER Time = { 0 };
	QueryPerformanceCounter( &Time );

	return Time.QuadPart / ( double ) this->InitTickPerSec;
}


/* ----- TimePiece::GetElapsedTime --------------------------------------------------- */
/* �T�v  : �O�t���[������̌o�ߎ��Ԃ�߂��B                                            */
/* �߂�l: ( double )�t���[���Ԃ̎���                                                  */
/* ----------------------------------------------------------------------------------- */
double TimePiece::GetDeltaTime( void )
{
	return this->DeltaTime;
}

/* ----- TimePiece::GetIsTimePieceStoped --------------------------------------------- */
/* �T�v  : �^�C�}�[���~�܂��Ă��邩�߂��B                                              */
/* �߂�l: ( bool ) �^�C�}�[���~�܂��Ă��邩                                           */
/* ----------------------------------------------------------------------------------- */
bool TimePiece::GetIsTimePieceStoped( void )
{
	return this->isTimerStopped;
}

/* ----- TimePiece::LimitThreadAffinityToCurrentProc --------------------------------- */
/* �T�v  : ���݂̗��p�X���b�h��1�̃v���Z�b�T(���݂̃X���b�h)�ɐ���                   */
/* �߂�l: �Ȃ�                                                                        */
/* ----------------------------------------------------------------------------------- */
//void TimePiece::LimitThreadAffinityToCurrentProc( void )
//{
//	HANDLE CurrentProcessHandle = GetCurrentProcess();
//
//	// Get the processor affinity mask for this process
//	DWORD_PTR ProcessAffinityMask = 0;
//	DWORD_PTR SystemAffinityMask = 0;
//
//	if( GetProcessAffinityMask( CurrentProcessHandle, &ProcessAffinityMask, &SystemAffinityMask ) != 0 && ProcessAffinityMask )
//	{
//		// Find the lowest processor that our process is allows to run against
//		DWORD_PTR dwAffinityMask = ( ProcessAffinityMask & ( ( ~ProcessAffinityMask ) + 1 ) );
//
//		// Set this as the processor that our thread must always run against
//		// This must be a subset of the process affinity mask
//		HANDLE CurrentThreadHandle = GetCurrentThread();
//		if( INVALID_HANDLE_VALUE != CurrentThreadHandle )
//		{
//			SetThreadAffinityMask( CurrentThreadHandle, ProcessAffinityMask );
//			CloseHandle( CurrentProcessHandle );
//		}
//	}
//	else
//	{
//		CloseHandle( CurrentProcessHandle );
//	}
//}

/* ----- TimePiece::GetAdjustedCurrentTime ------------------------------------------- */
/* �T�v  : �^�C�}�[����~���Ă���Β�~����,�ғ����Ă���Ό��ݎ��Ԃ�߂��B             */
/* �߂�l: ( LARGE_INTEGER ) �^�C�}�[����~���Ă���Β�~����,�ғ����Ă���Ό��ݎ���   */
/* ----------------------------------------------------------------------------------- */
LARGE_INTEGER TimePiece::GetAdjustedCurrentTime( void )
{
	LARGE_INTEGER Time;
	if( this->StopTime != 0 )
	{
		// �^�C�}�[����~���Ă���̂Œ�~���̃J�E���^
		Time.QuadPart = this->StopTime;
	}
	else
	{
		// �^�C�}�[�ғ����͌��݂̃J�E���^
		QueryPerformanceCounter( &Time );
	}

	return Time;
}

/* ----- TimePiece::Update ----------------------------------------------------------- */
/* �T�v  : ���Ԃ��v������B���t���[���ĂԂ��ƁB                                        */
/* �߂�l: ( void  ) �Ȃ�                                                              */
/* ----------------------------------------------------------------------------------- */
void TimePiece::Update( void )
{
	LARGE_INTEGER CurrentTime = GetAdjustedCurrentTime();
	LARGE_INTEGER currentFrequency;

	QueryPerformanceFrequency( &currentFrequency );
	DeltaTime = ( double ) ( CurrentTime.QuadPart - this->LastElapsedTime ) / InitTickPerSec;//( double ) currentFrequency.QuadPart;
	this->LastElapsedTime = CurrentTime.QuadPart;

	// TCHAR text[ 256 ];
	// sprintf( text, "Delta time = %.7f\n", (float) DeltaTime );
	// MyOutputDebugString( text );
	// ��������
	// �^�C�}�[�����m�ł��邱�Ƃ�ۏ؂��邽�߂ɁA�X�V���Ԃ��O�ɃN�����v����B
	// elapsed_time�́A�v���Z�b�T���ߓd���[�h�ɓ��邩,���炩�̌`�ŕʂ̃v���Z�b�T�ɃV���b�t��������,���͈̔͊O�ɂȂ�\��������.
	// ���,���C���X���b�h��SetThreadAffinityMask���Ăяo����,�ʂ̃v���Z�b�T�ɃV���b�t������Ȃ��悤�ɂ���K�v������.
	// ���̃��[�J�[�X���b�h��SetThreadAffinityMask���Ăяo���ׂ��ł͂Ȃ�,���C���X���b�h������W���ꂽ�^�C�}�[�f�[�^�̋��L�R�s�[���g�p���邱��.
}


/* +++++++++++++++ ���L<��> +++++++++++++++ */
/* ----- ���L<��> -----*/
// ���L<��>

/* ----- �֐��� XXXXXXXXXXXXXXXXXXXX ------------------------------------------------- */
/* �T�v XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  */
/* �߂�l: XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX�@�@�@�@�@�@�@�@�@�@�@*/
/* arg0: Type VariableName �T�v                                                        */
/* ----------------------------------------------------------------------------------- */

//ReturnType FunctionName( Type arg0... )
// {
//     /* ----- �����J�n ----- */
//     int XXXXX = NULL// �T�v
//     Func01() // ����T�v
//     a = Func02(){}
//         // �֐���
//         // �T�v: XXXX
//         // �ߒl: XXXXXX
//     Func03( arg0, // �^�@������ �T�v
//             arg1, // �^�@������ �T�v
//             arg2 ){}

// }