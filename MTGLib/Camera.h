#pragma once
#include "Main.h"
#include "ComponentEssential.h"
class Camera
	:
	public ComponentBase
{
public:
	Camera();
	~Camera();
private:
	RECT m_Viewport;
	XMMATRIX m_ViewMatrix;
	XMMATRIX m_InverseViewMatrix;
	XMMATRIX m_ProjectionMatrix;
public:
	void SetViewPort(LONG left, LONG top, LONG right, LONG bottom); // ���ꂢ��񂭂ˁBDX11Executor�ł�邩���߂邱�ƁB
	void PrepareForRendering();

	XMMATRIX& GetViewMatrix() { return m_ViewMatrix; }
	XMMATRIX& GetInverseViewMatrix() { return m_InverseViewMatrix; }
	XMMATRIX& GetProjectionMatrix() { return m_ProjectionMatrix; }
protected:
	void OnAwake() override;
	void OnUpdate(const float dt) override;
};