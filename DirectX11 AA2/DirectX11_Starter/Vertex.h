#ifndef VERTEX_H
#define VERTEX_H

#include <DirectXMath.h>
#include <utility>

// Data structure that constains various information to be used as vertices
struct Vertex
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT2 UV;
	DirectX::XMFLOAT3 Normal;
};

// Struct to match vertex shader's constant buffer
// You update one of these locally, then push it to the corresponding
// constant buffer on the device when it needs to be updated
struct VertexShaderConstantBuffer
{
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
};

// Text Stuff
struct FontType
{
	float left, right;
	int size;
};

struct VertexType
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texture;
};

struct PixelBufferType
{
	DirectX::XMFLOAT4 pixelColor;
};

struct SentenceType
{
	ID3D11Buffer *vertexBuffer, *indexBuffer;
	int vertexCount, indexCount, maxLength;
	float red, green, blue;
};

#endif