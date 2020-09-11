#pragma once
#include "Scene.h"
#include "Camera.h"
#include "TmpComponent.h"
#include "MeshHolder.h"
#include "MeshRenderer.h"
#include "ShaderHolder.h"
#include "RecompileShaderTest.h"
#include "GameWindow.h"

using namespace MyMath;
using namespace std;
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
		MeshHolder* meshHolder = OrderedSingleton<MeshHolder>::Instance();
		meshHolder->Load("data/MODEL/miku_01.obj");
		std::shared_ptr<Shader3D> shader = make_shared<Shader3D>();
		shader->Init("data/SHADER/VertexShader3D.hlsl", "data/SHADER/PixelShader3D.hlsl");



		std::shared_ptr<GameObject> obj = AddGameObject<GameObject>().lock();
		obj->AddComponent<TmpComponent>().lock()->Tmp();
		std::shared_ptr<MeshRenderer> renderer = obj->AddComponent<MeshRenderer>().lock();
		renderer->SetMeshData(meshHolder->GetData("data/MODEL/miku_01.obj"));
		std::string name = std::string("test");
		renderer->SetShader3D(shader);

		obj->AddComponentA<RecompileShaderTest>();

		// camera
		GameObject* cameraObject = AddGameObject<GameObject>().lock().get();
		Camera* camera = cameraObject->AddComponent<Camera>().lock().get();
		cameraObject->transform.lock()->SetLocalPosition(1.0f, 1.0f, 3.0f);
		cameraObject->transform.lock()->SetRotationLookAt(Vector3(0.0f, 0.0f, 0.0f));
		GameWindow* gameWindow = OrderedSingleton<GameWindow>::Instance();
		camera->SetViewPort(0, 0, gameWindow->Width(), gameWindow->Height());

	}
};