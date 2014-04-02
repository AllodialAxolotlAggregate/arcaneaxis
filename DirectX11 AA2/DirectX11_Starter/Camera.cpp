#include "Camera.h"

using namespace DirectX;

Camera* Camera::m_Instance = nullptr;
Camera* Camera::GetInstance()
{
	if(m_Instance == nullptr)
		m_Instance = new Camera();

	return m_Instance;
}

Camera::Camera()
{
	m_Position = XMFLOAT3(0, 0, -5);
	/*horizontalAngle = XM_PI;
	verticalAngle = 0.0f;
	initialFoV = 45.0f;*/
}

Camera::~Camera() {}

Camera Camera::operator += (XMFLOAT3 pos)
{
	m_Position.x += pos.x;
	m_Position.y += pos.y;
	m_Position.z += pos.z;

	return *this;
}

void Camera::ComputeMatrices()
{
	XMVECTOR pos = XMVectorSet(m_Position.x, m_Position.y, m_Position.z, 0.0);
	XMVECTOR target = XMVectorSet(0, 0, 0, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
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

void Camera::SetViewMatrix(DirectX::XMFLOAT4X4 otherViewMatrix) { m_ViewMatrix = otherViewMatrix; }
DirectX::XMFLOAT4X4 Camera::GetViewMatrix() { return m_ViewMatrix; }

void Camera::SetProjectionMatrix(DirectX::XMFLOAT4X4 otherProjectionMatrix) { m_ProjectionMatrix = otherProjectionMatrix; }
DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix() { return m_ProjectionMatrix; }

void Camera::SetOrthogoanlMatrix(DirectX::XMFLOAT4X4 otherOrthogoanlMatrix) { m_OrthogonalMatrix = otherOrthogoanlMatrix; }
DirectX::XMFLOAT4X4 Camera::GetOrthogonalMatrix() { return m_OrthogonalMatrix; }

void Camera::SetPosition(DirectX::XMFLOAT3 otherPosition) { m_Position = otherPosition; }
DirectX::XMFLOAT3& Camera::GetPosition() { return m_Position; }