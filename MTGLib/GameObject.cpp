#include "Scene.h"
#include "Transform.h"
#include "GameObject.h"

GameObject::GameObject(std::weak_ptr<Scene> scene)
	:
	GameObject(_T("Game Object"), scene)
{

}

GameObject::GameObject(const TCHAR* name, std::weak_ptr<Scene> scene)
	:
	Name_(name),
	m_Scene(scene)
{

}

GameObject::~GameObject()
{

}

void GameObject::UpdateComponents(const float dt) const
{
	for (auto component : m_Components)
	{
		component.second->OnUpdate(dt);
	}
}

void GameObject::AwakeComponents(void)
{
	for (auto component : m_ComponentsWaitingToAwake)
	{
		component.lock()->OnAwake();
	}

	m_ComponentsWaitingToAwake.clear();
}

void GameObject::StartComponents(void)
{
	for (auto component : m_ComponentsWaitingToStart)
	{
		component.lock()->OnStart();
	}

	m_ComponentsWaitingToStart.clear();
}