#ifndef VERTEX_H
#define VERTEX_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <utility>

// Data structure that constains various information to be used as vertices
struct OldVertex
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT2 UV;
};

struct FontVertex
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT2 UV;
};

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

// RasterTek Text Stuff
struct FontType
{
	float left, right;
	int size;
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

// Lighting
struct Light
{
	DirectX::XMFLOAT3 dir;
	float pad;
	DirectX::XMFLOAT3 pos;
	float range;
	DirectX::XMFLOAT3 att;
	float pad2;
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
};

struct cbPerFrame
{
	DirectX::XMFLOAT3 dir;
	float pad;
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
};

#endif