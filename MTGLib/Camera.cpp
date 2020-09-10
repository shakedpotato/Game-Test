#include "Main.h"
#include "OrderedSingleton.h"
#include "DX11Executor.h"
#include "RenderingExecuter.h"
#include "Camera.h"
#include "MTG.h"
#include "KeyInput.h"

using namespace std;
using namespace MyMath;
using namespace MTG;

Camera::Camera()
	:
	ComponentBase("Camera")
{

}

Camera::~Camera()
{

}

void Camera::SetViewPort(LONG left, LONG top, LONG right, LONG bottom)
{
	m_Viewport.left = left;
	m_Viewport.top = top;
	m_Viewport.right = right;
	m_Viewport.bottom = bottom;
}

void Camera::PrepareForRendering()
{
	// ビューポート設定
	D3D11_VIEWPORT dxViewport;
	dxViewport.Width = ( float ) ( m_Viewport.right - m_Viewport.left );
	dxViewport.Height = ( float ) ( m_Viewport.bottom - m_Viewport.top );
	dxViewport.MinDepth = 0.0f;
	dxViewport.MaxDepth = 1.0f;
	dxViewport.TopLeftX = ( float ) m_Viewport.left;
	dxViewport.TopLeftY = ( float ) m_Viewport.top;

	// マトリクスの設定
	// マルチスレッド処理しない前提。
	weak_ptr<GameObject> gameObject = GetGameObject();
	weak_ptr<Transform> transform = gameObject.lock()->GetComponent<Transform>();// マルチスレッド処理するならlock()のところ気を付けてね。

	ID3D11DeviceContext* context;
	context = OrderedSingleton<DX11Executor>::Instance()->GetDeviceContext();
	context->RSSetViewports(1, &dxViewport); // マルチビューポートに対応するときには変更の必要がありそう。

	const Vector3* position = transform.lock()->GetLocalPosition();
	const Vector3* rotation = transform.lock()->GetLocalRotation();
	// const XMFLOAT3* scale		= transform.lock()->GetScale(); カメラでのスケーリングはまた今度。

	// ビューマトリクス設定
	m_InverseViewMatrix = transform.lock()->GetRotationMatrix();
	// m_InvViewMtx = XMMatrixRotationRollPitchYaw(rotation->x, rotation->y, rotation->z);
	m_InverseViewMatrix *= XMMatrixTranslation(position->x, position->y, position->z);

	XMVECTOR det;
	m_ViewMatrix = XMMatrixInverse(&det, m_InverseViewMatrix); // 

	// プロジェクションマトリクス設定
	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(1.0f, dxViewport.Width / dxViewport.Height, 1.0f, 1000.0f);

}

void Camera::OnAwake()
{
	OrderedSingleton<RenderingExecuter>::Instance()->SetCamera(dynamic_pointer_cast<Camera>(shared_from_this()));
}

void Camera::OnUpdate(const float dt)
{
	KeyInput* key = OrderedSingleton<KeyInput>::Instance();
	Transform* transform = GetGameObject().lock()->transform.lock().get();
	
	float speed = 1.0f * dt;
	
	if (key->GetKeyPress('W'))
	{
		transform->AddLocalPosition(Vector3(0.0f, 0.0f, speed));
	}
	if (key->GetKeyPress('D'))
	{
		transform->AddLocalPosition(Vector3(speed, 0.0f, 0.0f));
	}
	if (key->GetKeyPress('S'))
	{
		transform->AddLocalPosition(Vector3(0.0f, 0.0f, -speed));
	}
	if (key->GetKeyPress('A'))
	{
		transform->AddLocalPosition(Vector3(-speed, 0.0f, 0.0f));
	}

	transform->SetRotationLookAt(Vector3(0.0f, 0.0f, 0.0f));

}