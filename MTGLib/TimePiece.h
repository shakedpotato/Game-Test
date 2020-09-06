// ===============================================================================================
// プロジェクト名: DirectX9game01172018 
// ファイル名: TikTimer.h
// 概要: 
// 作成日 XX/XX/XXXX Author: Hashimoto Yuto AT12E-275-29                                       
// 追記:                                                                                        
// MM/DD/YYYY Author: ○○                                                                      
// ・××の変更～～～～                                                                         
// ===============================================================================================

#ifndef _TIME_PIECE_H_
#define _TIME_PIECE_H_
#pragma once

#include "GameBasicSystem.h"

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
// 
// クラス: TimePiece 
// 注意点: GameSystemObjectテンプレートからインスタンスを取得すること。
// 
// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
class TimePiece
{
	/* ---------------------
		以下,時間はクロックの累積を示す.
	--------------------- */
private:
	friend class GameBasicSystemObject<TimePiece>;
	TimePiece();
public:
	void Reset( void );                            // システムタイマーのリセット
	void Start( void );                            // システムタイマーのスタート
	void Update( void );                           // 前フレームの時間などの更新
	void Stop( void );                             // システムタイマーのストップ
	void Put1msAhead( void );                      // システムタイマーを0.1秒進める
	double GetTime( void );                        // 計測時間の取得
	double GetAbsoluteTime( void );                // 現在時刻を取得
	double GetDeltaTime( void );                   // 前のフレームからの経過時間の取得
	bool GetIsTimePieceStoped( void );             // システムタイマーが止まっているか返す
	// void LimitThreadAffinityToCurrentProc( void ); // 現在のスレッドを1つのプロセッサ（現在のスレッド）に制限
private:
	bool isTimerStopped;       // ストップフラグ
	LONGLONG InitTickPerSec;   // １秒間の計測時間 // コンストラクタで一度初期化し,その後のフレームでは流用されてるので怪しい感じする.
	LONGLONG StopTime;         // ストップした時のカウンタ
	LONGLONG LastElapsedTime;  // 最後に記録したカウンタ
	LONGLONG BaseTime;         // 初期化時のカウンタ
	double DeltaTime;          // 前フレームからの経過時間。単位:秒
private:
	LARGE_INTEGER GetAdjustedCurrentTime( void );
};

#endif // _TIME_PIECE_H_