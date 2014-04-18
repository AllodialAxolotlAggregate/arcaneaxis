cbuffer perModel : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
};

struct VertexInputType
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
    

	// Change the position vector to be 4 units for proper matrix calculations.
    //input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    /*output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);*/
	matrix worldViewProj = mul(mul(world, view), projection);
	output.position = mul(float4(input.position, 1.0f), worldViewProj);
    
	// Store the texture coordinates for the pixel shader.
	//output.color = input.color;
	output.tex = input.tex;
    
    return output;
}