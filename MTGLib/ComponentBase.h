#pragma once
#include "CommonWinApp.h"
#include <memory>

class GameObject;

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
//
// クラス:  ComponentBase
// 概要  :	GameObjectクラスにアタッチされる全てに対するベースのクラス。
//			GameObject::AddComponentBase()などによってGameObjectにアタッチされる。
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
	ComponentBase( const ComponentBase& ) = delete; // コピーコンストラクタ
	ComponentBase& operator = ( const ComponentBase& ) = delete; // コピー代入演算子
	ComponentBase( ComponentBase&& ) = delete; // ムーブコンストラクタ
	ComponentBase& operator = ( const ComponentBase&& ) = delete; // ムーブ代入演算子
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
