#include "Scene.h"
#include "Scene_Tmp.h"
#include "SceneManager.h"

using namespace std;

SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{

}

void SceneManager::Init(void)
{
	AddSceneA<Scene_Tmp>("Tmp scene");
}

void SceneManager::Uninit( void )
{

}

void SceneManager::UpdateScenes( void )
{
	// m_UndestroyableScene->Update();
	for( auto scene : m_Scenes )
	{
		scene->AwakeAndStart();
	}

	for( auto scene : m_Scenes )
	{
		scene->Update();
	}
}