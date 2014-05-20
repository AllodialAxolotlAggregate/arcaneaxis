
struct Light
{
	float3 dir;
	float cone;
	float3 pos;
	float range;
	float3 att;
	float4 ambient;
	float4 diffuse;
};

// The constant buffer that holds our "per frame" data
// - Each frame you draw with this shader will need to 
//	adjust for changes
cbuffer perModel : register( b0 )
{
	Light light;
};

Texture2D myTexture : register(t0); // (that’s a zero)
SamplerState mySample : register(s0);

// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
	float4 worldPos		: POSITION;
	float2 texCoord		: TEXCOORD0;
	float3 normal		: NORMAL;
};

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{	
	//return myTexture.Sample(mySample, input.texCoord); //Use your variable names
	
	input.normal = normalize(input.normal);

	float4 diffuse = myTexture.Sample(mySample, input.texCoord);
	
	float3 finalColor = float3(0.0f, 0.0f, 0.0f);

	//Create the vector between light position and pixels position
	float3 lightToPixelVec = light.pos - input.worldPos;
	/*
	float3 lightToPixelVec = float3(0.0f, 0.0f, 0.0f);
	lightToPixelVec.x = light.pos.x - input.worldPos.x;
	lightToPixelVec.y = light.pos.y - input.worldPos.y;
	lightToPixelVec.z = light.pos.z - input.worldPos.z;
	*/

	//Find the distance between the light pos and pixel pos
	float d = length(lightToPixelVec);
	//d = 0.5f;
	
	//Create the ambient light
	float3 finalAmbient = diffuse * light.ambient;

	//If pixel is too far, return pixel color with ambient light
	if( d > light.range )
		return float4(finalAmbient, diffuse.a);
		
	//Turn lightToPixelVec into a unit length vector describing
	//the pixels direction from the lights position
	lightToPixelVec /= d; 
	
	//Calculate how much light the pixel gets by the angle
	//in which the light strikes the pixels surface
	float howMuchLight = dot(lightToPixelVec, input.normal);

	//If light is striking the front side of the pixel
	if( howMuchLight > 0.0f )
	{	
		//Add light to the finalColor of the pixel
		finalColor += howMuchLight * diffuse * light.diffuse;
		
		//Calculate Light's Falloff factor
		finalColor /= light.att[0] + (light.att[1] * d) + (light.att[2] * (d*d));

		//Calculate falloff from center to edge of pointlight cone
		finalColor *= pow(max(dot(-lightToPixelVec, light.dir), 0.0f), light.cone);
	}	
        
	//make sure the values are between 1 and 0, and add the ambient
	finalColor = saturate(finalColor + finalAmbient);
	

	//finalColor = diffuse * light.ambient;
	//finalColor += saturate(dot(light.dir, input.normal) * light.diffuse * diffuse);

	// Return final color
	return float4(finalColor, diffuse.a);
}