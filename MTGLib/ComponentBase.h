#pragma once
#include "CommonWinApp.h"
#include <memory>

class GameObject;

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
//
// �N���X:  ComponentBase
// �T�v  :	GameObject�N���X�ɃA�^�b�`�����S�Ăɑ΂���x�[�X�̃N���X�B
//			GameObject::AddComponentBase()�Ȃǂɂ����GameObject�ɃA�^�b�`�����B
//
// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

class ComponentBase
	:
	public std::enable_shared_from_this<ComponentBase>
{
	friend class GameObject;
	friend class Scene;
private:
	//  = delete;
	ComponentBase( const ComponentBase& ) = delete; // �R�s�[�R���X�g���N�^
	ComponentBase& operator = ( const ComponentBase& ) = delete; // �R�s�[������Z�q
	ComponentBase( ComponentBase&& ) = delete; // ���[�u�R���X�g���N�^
	ComponentBase& operator = ( const ComponentBase&& ) = delete; // ���[�u������Z�q
private:
	std::weak_ptr<GameObject> m_pGameObject;
public:
	tstring Name_;
public:
	ComponentBase();
	ComponentBase(const TCHAR* ComponentBaseName);

	virtual ~ComponentBase();
private:
	void RelateToGameObject(std::weak_ptr<GameObject> gameObject)
	{
		m_pGameObject = gameObject;
	}

public:
	std::weak_ptr<GameObject> GetGameObject() { return m_pGameObject; }

protected:
	std::shared_ptr<ComponentBase> GetSharedFromThisComponentBase() { return shared_from_this(); } 
private:

protected:
	virtual void OnAwake() {};
	virtual void OnStart() {};
	virtual void OnUpdate( const float dt ) {}
	virtual void OnDestroy() {}
public:
};
