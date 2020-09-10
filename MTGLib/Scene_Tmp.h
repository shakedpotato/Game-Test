#pragma once
#include "Scene.h"
#include "Camera.h"
#include "TmpComponent.h"
#include "MeshHolder.h"
#include "MeshRenderer.h"
#include "ShaderHolder.h"
#include "GameWindow.h"

using namespace MyMath;

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

		MeshHolder* meshHolder = OrderedSingleton<MeshHolder>::Instance();
		meshHolder->Load("data/MODEL/miku_01.obj");
		Shader3DHolder* shaderHolder = OrderedSingleton<Shader3DHolder>::Instance();
		shaderHolder->LoadShader("VertexShader3D.cso", "PixelShader3D.cso", "test");

		std::shared_ptr<GameObject> obj = AddGameObject<GameObject>().lock();
		obj->AddComponent<TmpComponent>().lock()->Tmp();
		std::shared_ptr<MeshRenderer> renderer = obj->AddComponent<MeshRenderer>().lock();
		renderer->SetMeshData(meshHolder->GetData("data/MODEL/miku_01.obj"));
		std::string name = std::string("test");
		renderer->SetShader3D(shaderHolder->GetShader(std::string("test")));

		GameObject* cameraObject = AddGameObject<GameObject>().lock().get();
		Camera* camera = cameraObject->AddComponent<Camera>().lock().get();

		int b = 0;
		cameraObject->transform.lock()->SetLocalPosition(1.0f, 1.0f, 3.0f);
		cameraObject->transform.lock()->SetRotationLookAt(Vector3(0.0f, 0.0f, 0.0f));
		GameWindow* gameWindow = OrderedSingleton<GameWindow>::Instance();

		camera->SetViewPort(0, 0, gameWindow->Width(), gameWindow->Height());

	}
};