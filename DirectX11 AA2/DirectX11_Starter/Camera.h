#ifndef CAMERA_H
#define CAMERA_H

#include <d3d11.h>
#include <DirectXMath.h>

struct Camera
{
public:
	Camera();
	Camera(DirectX::XMFLOAT3 aPosition);
	Camera(DirectX::XMFLOAT3 aPosition, DirectX::XMFLOAT3 aTarget, DirectX::XMFLOAT3 anUp);
	~Camera();

	void ComputeMatrices();
	void OnResize(float ar);

	static Camera* GetInstance();
	//Camera operator += (DirectX::XMFLOAT3 aPosition);

#pragma region Get/Sets

	void SetViewMatrix(DirectX::XMFLOAT4X4 otherViewMatrix) { m_ViewMatrix = otherViewMatrix; }
	DirectX::XMFLOAT4X4 GetViewMatrix() { return m_ViewMatrix; }
	__declspec(property(get = GetViewMatrix, put = SetViewMatrix)) DirectX::XMFLOAT4X4 r_ViewMatrix;

	void SetProjectionMatrix(DirectX::XMFLOAT4X4 otherProjectionMatrix) { m_ProjectionMatrix = otherProjectionMatrix; }
	DirectX::XMFLOAT4X4 GetProjectionMatrix() { return m_ProjectionMatrix; }
	__declspec(property(get = GetProjectionMatrix, put = SetProjectionMatrix)) DirectX::XMFLOAT4X4 r_ProjectionMatrix;

	void SetOrthogoanlMatrix(DirectX::XMFLOAT4X4 otherOrthogoanlMatrix) { m_OrthogonalMatrix = otherOrthogoanlMatrix; }
	DirectX::XMFLOAT4X4 GetOrthogonalMatrix() { return m_OrthogonalMatrix; }
	__declspec(property(get = GetOrthogonalMatrix, put = SetOrthogoanlMatrix)) DirectX::XMFLOAT4X4 r_OrthogoanlMatrix;

	void SetPosition(DirectX::XMFLOAT3 otherPosition) { m_Position = otherPosition; }
	DirectX::XMFLOAT3& GetPosition() { return m_Position; }
	__declspec(property(get = GetPosition, put = SetPosition)) DirectX::XMFLOAT3 r_Position;

	void SetTarget(DirectX::XMFLOAT3 otherTarget) { m_Target = otherTarget; }
	DirectX::XMFLOAT3& GetTarget() { return m_Target; }
	__declspec(property(get = GetTarget, put = SetTarget)) DirectX::XMFLOAT3 r_Target;

	void SetUp(DirectX::XMFLOAT3 otherUp) { m_Up = otherUp; }
	DirectX::XMFLOAT3& GetUp() { return m_Up; }
	__declspec(property(get = GetUp, put = SetUp)) DirectX::XMFLOAT3 r_Up;

#pragma endregion

private:
	DirectX::XMFLOAT4X4 m_ViewMatrix;
	DirectX::XMFLOAT4X4 m_ProjectionMatrix;
	DirectX::XMFLOAT4X4 m_OrthogonalMatrix;

	DirectX::XMFLOAT3 m_Position;
	DirectX::XMFLOAT3 m_Target;
	DirectX::XMFLOAT3 m_Up;

	static Camera* m_Instance;
};

#endif