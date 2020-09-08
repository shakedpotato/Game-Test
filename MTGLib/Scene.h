#pragma once
#include <list>
#include <map>
#include <memory>
#include "OrderedSingleton.h"
#include "Timer.h"
#include "ComponentBase.h"
#include "GameObject.h"
#include "Transform.h"

class GameObject;
// class Component;

class Scene
	:
	public std::enable_shared_from_this<Scene>
{
private:
	Scene()
		:
		Name_(_T("Scene")),
		isEnableUpdate_(true)
	{
	}
public:
	Scene(const TCHAR* name)
		:
		Name_(name),
		isEnableUpdate_(true)
	{

	}
	virtual ~Scene()
	{

	}
public:
	tstring Name_;
	bool isEnableUpdate_;
private:
	std::list<std::shared_ptr<GameObject>>		m_GameObjects;
public:
	virtual void Init(void) = 0;
	virtual void Uninit(void)
	{
		for (auto obj : m_GameObjects)
		{
			// obj->Uninit();
		}
	}

	virtual void AwakeAndStart(void)
	{
		for (auto obj : m_GameObjects)
		{
			obj->AwakeComponents();
		}

		for (auto obj : m_GameObjects)
		{
			obj->StartComponents();
		}
	}

	virtual void Update(void)
	{
		if (isEnableUpdate_)
			return;
		
		float dt = OrderedSingleton<DeltaTimer>::Instance()->deltaTime();

		for (auto obj : m_GameObjects)
		{
			obj->UpdateComponents(dt);
			obj->Update();
		}
	}

	template<typename T>
	std::weak_ptr<GameObject> AddGameObject(void)
	{
		std::shared_ptr<T> obj = std::make_shared<T>(shared_from_this());
		
		m_GameObjects.emplace_back(obj);

		obj->Init();
		obj->transform = obj->AddComponent<Transform>();

		return m_GameObjects.back();
	}

	template<typename T>
	std::weak_ptr<T> AddGameObjectT(void)
	{
		return std::dynamic_pointer_cast<T>(AddGameObject<T>());
	}
};

