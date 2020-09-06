// ===============================================================================================              
// 概要:                                                                                      
//                                                                                              
// 作成日 XX/XX/XXXX Author: Hashimoto Yuto                             
// ===============================================================================================

#ifndef _GAME_SYSTEM_H_
#define _GAME_SYSTEM_H_

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
// 
// インクルードファイル
// 
// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

#include "CommonWinApp.h"

#include <cassert>
#include <mutex>
#include <memory>
#include <list>

#include <tchar.h>
#include <typeinfo>

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
//
// クラス: GameSystemFinalizer
// 概要  : GameSystemObjectの終了処理を一括で呼ぶためのクラス。
//         シングルトンなゲームの機関システムを一括で終了処理し、生成と逆順に削除する。
//
// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-


class GameBasicSystemFinalizer
{
public:
	typedef void(*FinalizerFunc)();

	static void AddFinalizer(FinalizerFunc func);
	static void FinalizeAll(void);

private:
	static std::list<FinalizerFunc> m_Finalizers;
};


// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
//
// クラス: GameBasicSystemObject
// 概要  : シングルトンパターンのテンプレート。
//         生成時にGameBasicSystemFinalizerに登録され、最後に生成と逆順に破棄される。
//		   シングルトンで破棄をアプリケーションの最後に一括で行ってもよいクラスに用いる。
//		   これで作るクラスを他で生成できないようにしたい。 
//         ---> これを利用して生成したいクラスでデフォルトコンストラクタをprivateにし、
//              GameBasicSystemObject<T>をfriendクラスにすればOK(?)。
//
// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

template <typename T>
class GameBasicSystemObject final
{
private:
	static std::once_flag m_InitFlag;
	static T* m_Instance;
public:
	static T* Instance(void)
	{
		std::call_once(m_InitFlag, Init); // 一度だけ呼ばれることを保証
		assert(m_Instance);
		return m_Instance;
	}
private:
	GameBasicSystemObject() {};
public:
	~GameBasicSystemObject()
	{
		// GameBasicSystemFinalizer::RemoveFinalizer(&GameSystemObject<T>::Destroy);
	}

/* // たぶんいらない
public:
	GameBasicSystemObject(const GameBasicSystemObject&) = delete;
	GameSystemObject& operator=(const GameSystemObject&) = delete;
	GameSystemObject(GameSystemObject&&) = delete;
	GameSystemObject& operator=(GameSystemObject&&) = delete;
*/
private:
	static void Init(void)
	{
		MyOutputDebugString(_T("GameBasicSystemObject::Init(). Class name = %s\n"), typeid(m_Instance).name());
		m_Instance = new T;

		assert(m_Instance);

		GameBasicSystemFinalizer::AddFinalizer(&GameBasicSystemObject<T>::Destroy);
	}

	static void Destroy(void)
	{
		MyOutputDebugString(_T("GameBasicSystemObject::Destroy(). Class name = %s\n"), typeid(m_Instance).name());
		delete m_Instance;
		m_Instance = nullptr;
	}
};

template <typename T> std::once_flag GameBasicSystemObject<T>::m_InitFlag;
template <typename T> T* GameBasicSystemObject<T>::m_Instance = nullptr;

#endif // _GAME_SYSTEM_H_