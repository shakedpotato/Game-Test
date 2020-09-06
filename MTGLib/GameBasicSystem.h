// ===============================================================================================              
// �T�v:                                                                                      
//                                                                                              
// �쐬�� XX/XX/XXXX Author: Hashimoto Yuto                             
// ===============================================================================================

#ifndef _GAME_SYSTEM_H_
#define _GAME_SYSTEM_H_

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
// 
// �C���N���[�h�t�@�C��
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
// �N���X: GameSystemFinalizer
// �T�v  : GameSystemObject�̏I���������ꊇ�ŌĂԂ��߂̃N���X�B
//         �V���O���g���ȃQ�[���̋@�փV�X�e�����ꊇ�ŏI���������A�����Ƌt���ɍ폜����B
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
// �N���X: GameBasicSystemObject
// �T�v  : �V���O���g���p�^�[���̃e���v���[�g�B
//         ��������GameBasicSystemFinalizer�ɓo�^����A�Ō�ɐ����Ƌt���ɔj�������B
//		   �V���O���g���Ŕj�����A�v���P�[�V�����̍Ō�Ɉꊇ�ōs���Ă��悢�N���X�ɗp����B
//		   ����ō��N���X�𑼂Ő����ł��Ȃ��悤�ɂ������B 
//         ---> ����𗘗p���Đ����������N���X�Ńf�t�H���g�R���X�g���N�^��private�ɂ��A
//              GameBasicSystemObject<T>��friend�N���X�ɂ����OK(?)�B
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
		std::call_once(m_InitFlag, Init); // ��x�����Ă΂�邱�Ƃ�ۏ�
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

/* // ���Ԃ񂢂�Ȃ�
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