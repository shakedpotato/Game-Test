// ===============================================================================================
// プロジェクト名: MyGame
// ファイル名: TikTimer.cpp
// 概要: Windowsの高分解能を用いて、アプリケーションの時間の経過時間を計測する。
// 作成日 ??/??/???? Author: Hashimoto Yuto AT12E-275-29                                    
// ===============================================================================================

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
// 
// インクルード
// 
// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
#include "CommonWinApp.h"
#include "TimePiece.h"

//
// コンストラクタ
//
TimePiece::TimePiece()
{
	this->isTimerStopped  = true; 
	this->InitTickPerSec = 0;
	this->StopTime        = 0;    
	this->LastElapsedTime = 0;    
	this->BaseTime        = 0;    
	this->DeltaTime       = 0.0f;

	// 高分解能パフォーマンスカウンタ周波数の取得
	LARGE_INTEGER tmpTicksPerSec = { 0 };
	QueryPerformanceFrequency( &tmpTicksPerSec );
	this->InitTickPerSec = tmpTicksPerSec.QuadPart;

}

/* ----- TimePiece::Reset ------------------------------------------------------------ */
/* 概要  :タイマーのリセット                                                           */
/* 戻り値: なし                                                                        */
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
/* 概要  :タイマーの初期化と計測開始。                                                 */
/*        本クラスの利用前に一度だけ必ず呼ぶこと。                                     */
/* 戻り値: なし                                                                        */
/* ----------------------------------------------------------------------------------- */
void TimePiece::Start( void )
{
	// 現在時刻を取得
	LARGE_INTEGER Time = { 0 };
	QueryPerformanceCounter( &Time );

	this->StopTime = 0;
	this->BaseTime = this->LastElapsedTime = Time.QuadPart;
	this->isTimerStopped = false;
}

/* ----- TimePiece::Stop ------------------------------------------------------------- */
/* 概要  : タイマーを停止する。                                                        */
/* 戻り値: なし                                                                        */
/* ----------------------------------------------------------------------------------- */
void TimePiece::Stop( void )
{
	if( this->isTimerStopped ) { return; }

	LARGE_INTEGER Time = { 0 };
	QueryPerformanceCounter( &Time );
	// 停止時間を記録して停止フラグを立てる
	this->LastElapsedTime = this->StopTime = Time.QuadPart;
	this->isTimerStopped = true;
}

/* ----- TimePiece::Put1msAhead ------------------------------------------------------ */
/* 概要  : 停止時間を1ms分の周波数分増やす。                                           */
/* 戻り値: なし                                                                        */
/* ----------------------------------------------------------------------------------- */
void TimePiece::Put1msAhead( void )
{
	this->StopTime += this->InitTickPerSec / 10;
}

/* ----- TimePiece::GetTime ---------------------------------------------------------- */
/* 概要  : Start()が呼ばれてからの経過時間を戻す。                                     */
/* 戻り値: ( double ) Start()呼び出しからの経過時間                                    */
/* ----------------------------------------------------------------------------------- */
double TimePiece::GetTime( void )
{
	LARGE_INTEGER Time = GetAdjustedCurrentTime();
	
	return (double) ( Time.QuadPart - this->BaseTime ) / ( double ) this->InitTickPerSec;
}

/* ----- TimePiece::GetAbsoluteTime -------------------------------------------------- */
/* 概要  : 現在の時間を取得(？)                                                        */
/* 戻り値: ( double )現在の時間                                                        */
/* ----------------------------------------------------------------------------------- */
double TimePiece::GetAbsoluteTime( void )
{
	LARGE_INTEGER Time = { 0 };
	QueryPerformanceCounter( &Time );

	return Time.QuadPart / ( double ) this->InitTickPerSec;
}


/* ----- TimePiece::GetElapsedTime --------------------------------------------------- */
/* 概要  : 前フレームからの経過時間を戻す。                                            */
/* 戻り値: ( double )フレーム間の時間                                                  */
/* ----------------------------------------------------------------------------------- */
double TimePiece::GetDeltaTime( void )
{
	return this->DeltaTime;
}

/* ----- TimePiece::GetIsTimePieceStoped --------------------------------------------- */
/* 概要  : タイマーが止まっているか戻す。                                              */
/* 戻り値: ( bool ) タイマーが止まっているか                                           */
/* ----------------------------------------------------------------------------------- */
bool TimePiece::GetIsTimePieceStoped( void )
{
	return this->isTimerStopped;
}

/* ----- TimePiece::LimitThreadAffinityToCurrentProc --------------------------------- */
/* 概要  : 現在の利用スレッドを1つのプロセッサ(現在のスレッド)に制限                   */
/* 戻り値: なし                                                                        */
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
/* 概要  : タイマーが停止していれば停止時間,稼働していれば現在時間を戻す。             */
/* 戻り値: ( LARGE_INTEGER ) タイマーが停止していれば停止時間,稼働していれば現在時間   */
/* ----------------------------------------------------------------------------------- */
LARGE_INTEGER TimePiece::GetAdjustedCurrentTime( void )
{
	LARGE_INTEGER Time;
	if( this->StopTime != 0 )
	{
		// タイマーが停止しているので停止時のカウンタ
		Time.QuadPart = this->StopTime;
	}
	else
	{
		// タイマー稼働中は現在のカウンタ
		QueryPerformanceCounter( &Time );
	}

	return Time;
}

/* ----- TimePiece::Update ----------------------------------------------------------- */
/* 概要  : 時間を計測する。毎フレーム呼ぶこと。                                        */
/* 戻り値: ( void  ) なし                                                              */
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
	// メモ書き
	// タイマーが正確であることを保証するために、更新時間を０にクランプする。
	// elapsed_timeは、プロセッサが節電モードに入るか,何らかの形で別のプロセッサにシャッフルされると,この範囲外になる可能性がある.
	// よっ,メインスレッドはSetThreadAffinityMaskを呼び出して,別のプロセッサにシャッフルされないようにする必要がある.
	// 他のワーカースレッドはSetThreadAffinityMaskを呼び出すべきではなく,メインスレッドから収集されたタイマーデータの共有コピーを使用すること.
}


/* +++++++++++++++ 注記<大> +++++++++++++++ */
/* ----- 注記<中> -----*/
// 注記<小>

/* ----- 関数名 XXXXXXXXXXXXXXXXXXXX ------------------------------------------------- */
/* 概要 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  */
/* 戻り値: XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX　　　　　　　　　　　*/
/* arg0: Type VariableName 概要                                                        */
/* ----------------------------------------------------------------------------------- */

//ReturnType FunctionName( Type arg0... )
// {
//     /* ----- 処理開始 ----- */
//     int XXXXX = NULL// 概要
//     Func01() // 動作概要
//     a = Func02(){}
//         // 関数名
//         // 概要: XXXX
//         // 戻値: XXXXXX
//     Func03( arg0, // 型　引数名 概要
//             arg1, // 型　引数名 概要
//             arg2 ){}

// }