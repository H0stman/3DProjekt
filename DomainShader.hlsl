Texture2D HeightTexture                 : register(t0);
SamplerState LinearSampler : SAMPLER    : register(s0);

cbuffer constantBuffer : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 projection;
    float4x4 lightVP;
    float4x4 lightWVP;
    float3 camerapos;
};

struct HS_POINT_OUTPUT
{
	float3 inPositionWS     : POSITIONWS;
	float2 inTexCoord       : TEXCOORD;
	float3 inNormalWS       : NORMALWS;
    float3 inTangentWS      : TANGENTWS;
    float3 inBiTangentWS    : BITANGENTWS;
};

struct HS_PATCH_OUTPUT
{
	float Edges[3]	: SV_TessFactor;
	float Inside	: SV_InsideTessFactor;
};

struct DS_OUTPUT
{
	float4 outPositionCS    : SV_POSITION;
	float2 outTexCoord      : TEXCOORD;
	float3 outNormalWS      : NORMALWS;
	float3 outPositionWS    : POSITIONWS;
    float3 outTangentWS : TANGENTWS;
    float3 outBiTangentWS : BITANGENTWS;
    float4 outPositionLightCS : POSITIONLIGHTCS;
};

[domain("tri")]
DS_OUTPUT ds_main(const OutputPatch<HS_POINT_OUTPUT, 3> TriPatch, float3 Coords : SV_DomainLocation, HS_PATCH_OUTPUT input)
{
	DS_OUTPUT output;

	// Interpolate world space position
    float3 vWorldPos = Coords.x * TriPatch[0].inPositionWS + Coords.y * TriPatch[1].inPositionWS + Coords.z * TriPatch[2].inPositionWS;
    output.outPositionWS = vWorldPos;
	
	// Calculate the interpolated normal vector
	output.outNormalWS  = Coords.x * TriPatch[0].inNormalWS + Coords.y * TriPatch[1].inNormalWS + Coords.z * TriPatch[2].inNormalWS;

	// Normalize the vector length for use in displacement
	output.outNormalWS = normalize(output.outNormalWS);

	// Calculate the interpolated texture coordinates
    output.outTexCoord = Coords.x * TriPatch[0].inTexCoord + Coords.y * TriPatch[1].inTexCoord + Coords.z * TriPatch[2].inTexCoord;
	
    // Calculate the interpolated tangent coordinates
    output.outTangentWS = Coords.x * TriPatch[0].inTangentWS + Coords.y * TriPatch[1].inTangentWS + Coords.z * TriPatch[2].inTangentWS;

	//Calculate the interpolated bitangent coordinates
    output.outBiTangentWS = Coords.x * TriPatch[0].inBiTangentWS + Coords.y * TriPatch[1].inBiTangentWS + Coords.z * TriPatch[2].inBiTangentWS;
    
    // Calculate MIP level to fetch normal from
    float fHeightMapMIPLevel = clamp((distance(vWorldPos, camerapos) - 20.0f) / 20.0f, 0.0f, 3.0f);
    
    // Sample the height map to know how much displacement to implement
    float4 texHeight = HeightTexture.SampleLevel(LinearSampler, output.outTexCoord, fHeightMapMIPLevel);
    
    // Perform displacement
    // fScale determines max world space offset
    // Displacement performed along interpolated normal vector
    const float fScale = 0.9f;
    output.outPositionWS = output.outPositionWS + output.outNormalWS * texHeight.r * fScale;

    output.outPositionCS = mul(float4(output.outPositionWS, 1.0f), view);
    output.outPositionCS = mul(float4(output.outPositionCS), projection);
    output.outPositionLightCS = mul(float4(output.outPositionWS, 1.0f), lightVP);

	return output;
}