#pragma once

#include <unordered_map>
#include <vector>
#include <typeinfo>
#include <memory>

#include "CommonWinApp.h"

class ComponentBase;
class Transform;
class Scene;

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
//
// �N���X:  GameObject
// �T�v  :	�V�[�����ɂČŒ胋�[�`�����s���I�u�W�F�N�g�̃X�[�p�[�N���X�B
//			�X�̋@�\��Component�N���X�ɂčs���B
//
// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

class GameObject
	:
	public std::enable_shared_from_this<GameObject>
{
	friend class Scene;
private:
	GameObject(const GameObject&) = delete; // �R�s�[�R���X�g���N�^
	GameObject& operator = (const GameObject&) = delete; // �R�s�[������Z�q
	GameObject(GameObject&&) = delete; // ���[�u�R���X�g���N�^
	GameObject& operator = (const GameObject&&) = delete; // ���[�u������Z�q
public:
	GameObject(std::weak_ptr<Scene> scene);
	GameObject(const TCHAR* name, std::weak_ptr<Scene> scene);
	virtual ~GameObject();
protected:
	std::shared_ptr<GameObject> GetSharedfromthisGameObject() { return shared_from_this(); }
protected:
	std::weak_ptr<Scene> m_Scene;
	std::unordered_map<size_t, std::shared_ptr<ComponentBase>> m_Components; // Key: typeid(ComponentType).hash_code()
	std::list<std::weak_ptr<ComponentBase>> m_ComponentsWaitingToAwake;
	std::list<std::weak_ptr<ComponentBase>> m_ComponentsWaitingToStart;
public:
	tstring Name_;
	std::weak_ptr<Transform> transform;
public:
	virtual void Init(void) {}
	virtual void Update(void) {}
	virtual void Draw(void) {}
	virtual void Uninit(void) {}

	virtual void AwakeComponents(void) final;
	virtual void StartComponents(void) final;
	virtual void UpdateComponents(const float dt) const final;

	template<typename T>
	void AddComponentA() // return std::weak_ptr �� C26444 NO_UNNAMED_RAII_OBJECTS
	{
		tstring text = Name_.c_str();
		text += _T("->AddComponent<");
		text += _T(typeid(T).name());
		text += _T(">()\n");


		size_t i = typeid(T).hash_code();
		if (m_Components.find(i) != m_Components.end())
		{
			tstring text = _T("You are trying to add a component already contained to.");
			text += Name_.c_str();
			text += _T(".\n");
			text += _T("component type is ");
			text += _T(typeid(T).name());
			text += _T(".\n");

			MyOutputDebugString(text.c_str());
			assert(false);
		}

		size_t hash = typeid(T).hash_code();

		m_Components[hash] = std::make_shared<T>();
		m_Components[hash]->RelateToGameObject(GetSharedfromthisGameObject());
		m_ComponentsWaitingToAwake.push_back(m_Components[hash]);
		m_ComponentsWaitingToStart.push_back(m_Components[hash]);
		// m_Components[ hash ]->CallOnAwake();
	}

	template<typename T>
	std::weak_ptr<T> AddComponent() // return std::weak_ptr �� C26444 NO_UNNAMED_RAII_OBJECTS
	{
		AddComponentA<T>();

		return std::dynamic_pointer_cast<T>(m_Components[typeid(T).hash_code()]);
	}

	template<typename T>
	std::weak_ptr<T> GetComponent()
	{
		size_t i = typeid(T).hash_code();

		if (m_Components.find(i) == m_Components.end())
		{
			tstring text = _T("You are trying to get a component not contained.");
			text += Name_.c_str();
			text += _T(".\n");
			text += _T("component type is ");
			text += _T(typeid(T).name());
			text += _T(".\n");

			MyOutputDebugString(text.c_str());
			assert(false);
		}

		return std::dynamic_pointer_cast<T>(m_Components[typeid(T).hash_code()]);
	}

	template<typename T>
	void RemoveComponent()
	{
		m_Components[typeid(T).hash_code()]->OnDestroy();
		m_Components.erase(typeid(T).hash_code());
	}


	virtual std::weak_ptr<Scene> GetScene()const final { return m_Scene; }
};


