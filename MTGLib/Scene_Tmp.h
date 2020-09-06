#pragma once
#include "Scene.h"
#include "TmpComponent.h"

class Scene_Tmp
	:
	public Scene
{
public:
	Scene_Tmp(const TCHAR* name)
		:
		Scene(name)
	{

	}
public:
	void Init()override
	{
		int a = 0;

		std::weak_ptr<GameObject> obj = AddGameObject<GameObject>();
		obj.lock()->AddComponent<TmpComponent>().lock()->Tmp();

	}
};