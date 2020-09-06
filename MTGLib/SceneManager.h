#pragma once
#include "GameBasicSystem.h"

#include <vector>
#include <memory>

#include "Scene.h"

class SceneManager final
{
private:
	SceneManager();
	friend class GameBasicSystemObject<SceneManager>;
	~SceneManager();

private:
	GraphicRenderingExecuter*	m_Renderer;
	std::vector<std::shared_ptr<Scene>>	m_Scenes;
	// std::shared_ptr<Scene> m_UndestroyableScene;
public:
	void Init( void );
	void Uninit( void );
	void UpdateScenes( void );
public:
	// ‚½‚Ô‚ñ‚¢‚ç‚È‚¢•û
	template<typename T>
	std::weak_ptr<T> AddSceneT( const TCHAR* name )
	{
		m_Scenes.emplace_back( std::make_shared<T>( name ) );
		m_Scenes.back()->Init();
		return std::dynamic_pointer_cast< T >( m_Scenes.back() );
	}

	template<typename T>
	std::weak_ptr<Scene> AddScene( const TCHAR* name )
	{
		m_Scenes.emplace_back( std::make_shared<T>( name ) );
		m_Scenes.back()->Init();
		return m_Scenes.back();
	}
	
	template<typename T>
	void AddSceneA(const TCHAR* name)
	{
		m_Scenes.emplace_back( std::make_shared<T>( name ) );
		m_Scenes.back()->Init();
	}

	std::weak_ptr<Scene> GetSceneByName(const TCHAR* name)
	{
		auto itr = std::find_if(m_Scenes.begin(), m_Scenes.end(),
								[ & ](std::shared_ptr<Scene> s) {
									return !strcmp(s->Name_.c_str(), name);
								});

		return ( itr != m_Scenes.end() ) ? *itr : std::shared_ptr<Scene>(nullptr);
	}

	std::weak_ptr<Scene> GetSceneByBuildIndex(size_t index)
	{
		return ( m_Scenes.size() < index ) ? m_Scenes[ index ] : std::shared_ptr<Scene>(nullptr);
	}
};

