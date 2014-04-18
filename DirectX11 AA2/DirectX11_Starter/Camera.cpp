#include "Camera.h"
#define DEFAULT_POSITION XMFLOAT3(0, 0, -5)
#define DEFAULT_TARGET XMFLOAT3(0, 0, 0)
#define DEFAULT_UP XMFLOAT3(0, 1, 0)

using namespace DirectX;

Camera* Camera::m_Instance = nullptr;
Camera* Camera::GetInstance()
{
	if(m_Instance == nullptr)
		m_Instance = new Camera();

	return m_Instance;
}

Camera::Camera() :
	m_Position(DEFAULT_POSITION),
	m_Target(DEFAULT_TARGET),
	m_Up(DEFAULT_UP)
{
}

Camera::Camera(XMFLOAT3 aPosition) :
	m_Position(aPosition),
	m_Target(DEFAULT_TARGET),
	m_Up(DEFAULT_UP)
{
}

Camera::Camera(XMFLOAT3 aPosition, XMFLOAT3 aTarget, XMFLOAT3 anUp) :
	m_Position(aPosition),
	m_Target(aTarget),
	m_Up(anUp)
{
}

Camera::~Camera() {}

//Camera Camera::operator += (XMFLOAT3 pos)
//{
//	m_Position.x += pos.x;
//	m_Position.y += pos.y;
//	m_Position.z += pos.z;
//
//	return *this;
//}

void Camera::ComputeMatrices()
{
	XMVECTOR pos = XMVectorSet(m_Position.x, m_Position.y, m_Position.z, 0.0);
	XMVECTOR target = XMVectorSet(m_Target.x, m_Target.y, m_Target.z, 0);
	XMVECTOR up = XMVectorSet(m_Up.x, m_Up.y, m_Up.z, 0);
	XMMATRIX V = XMMatrixLookAtLH(pos, target, up);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(V));
}

void Camera::OnResize(float ar)
{
	// Update our projection matrix since the window size changed
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,
		ar,
		0.1f,
		100.0f);
	XMStoreFloat4x4(&m_ProjectionMatrix, XMMatrixTranspose(P));
}