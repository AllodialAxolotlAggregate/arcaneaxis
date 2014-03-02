#ifndef VERTEX_H
#define VERTEX_H

#include <DirectXMath.h>
#include <utility>

struct Vertex
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 Color;
	DirectX::XMFLOAT2 UV;
};

#endif