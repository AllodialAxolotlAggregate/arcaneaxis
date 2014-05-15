
// The constant buffer that holds our "per model" data
// - Each object you draw with this shader will probably have
//   slightly different data (at least for the world matrix)
cbuffer perModel : register( b0 )
{
	matrix world;
	matrix view;
	matrix projection;
};

// Defines what kind of data to expect as input
// - This should match our input layout!
struct VertexShaderInput
{
	float3 position		: POSITION;
	float4 color		: COLOR;
	float2 texCoord		: TEXCOORD0;
	float3 normal		: NORMAL;
};

// Defines the output data of our vertex shader
// - At a minimum, you'll need an SV_POSITION
// - Should match the pixel shader's input
struct VertexToPixel
{
	float4 position		: SV_POSITION;	// System Value Position - Has specific meaning to the pipeline!
	float4 color		: COLOR;
	float4 worldPos		: POSITION;
	float2 texCoord		: TEXCOORD0;
	float3 normal		: NORMAL;
};

// The entry point for our vertex shader
VertexToPixel main( VertexShaderInput input )
{	
	// Set up output
	VertexToPixel output;

	// Calculate output position
	matrix worldViewProj = mul(mul(world, view), projection);
	output.position = mul(float4(input.position, 1.0f), worldViewProj);

	output.color = input.color;

	output.worldPos = mul(input.position, world);

	// normal
	output.normal = mul(input.normal, world);

	// texCoord
	output.texCoord = input.texCoord;

	return output;
}