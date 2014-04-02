#ifndef CAMERA_H
#define CAMERA_H

#include <d3d11.h>
#include <DirectXMath.h>

struct Camera
{
public:
	Camera();
	~Camera();

	void ComputeMatrices();
	void OnResize(float);

	static Camera* GetInstance();
	Camera operator += (DirectX::XMFLOAT3);

#pragma region Get/Sets

	void SetViewMatrix(DirectX::XMFLOAT4X4);
	DirectX::XMFLOAT4X4 GetViewMatrix();
	__declspec(property(get = GetViewMatrix, put = SetViewMatrix)) DirectX::XMFLOAT4X4 r_ViewMatrix;

	void SetProjectionMatrix(DirectX::XMFLOAT4X4);
	DirectX::XMFLOAT4X4 GetProjectionMatrix();
	__declspec(property(get = GetProjectionMatrix, put = SetProjectionMatrix)) DirectX::XMFLOAT4X4 r_ProjectionMatrix;

	void SetOrthogoanlMatrix(DirectX::XMFLOAT4X4);
	DirectX::XMFLOAT4X4 GetOrthogonalMatrix();
	__declspec(property(get = GetOrthogonalMatrix, put = SetOrthogoanlMatrix)) DirectX::XMFLOAT4X4 r_OrthogoanlMatrix;

	void SetPosition(DirectX::XMFLOAT3);
	DirectX::XMFLOAT3& GetPosition();
	__declspec(property(get = GetPosition, put = SetPosition)) DirectX::XMFLOAT3 r_Position;

#pragma endregion

private:
	DirectX::XMFLOAT4X4 m_ViewMatrix;
	DirectX::XMFLOAT4X4 m_ProjectionMatrix;
	DirectX::XMFLOAT4X4 m_OrthogonalMatrix;

	DirectX::XMFLOAT3 m_Position;
	/*float horizontalAngle;
	float verticalAngle;
	float initialFoV;*/

	static Camera* m_Instance;
};

#endif