struct VS_OUTPUT
{
	float3 inPositionWS : POSITIONWS;
	float2 inTexCoord : TEXCOORD;
	float3 inNormalWS : NORMALWS;
    float3 inTangentWS : TANGENTWS;
    float3 inBiTangentWS : BITANGENTWS;
};

struct HS_POINT_OUTPUT
{
	float3 outPositionWS : POSITIONWS;
	float2 outTexCoord : TEXCOORD;
	float3 outNormalWS : NORMALWS;
    float3 outTangentWS : TANGENTWS;
    float3 outBiTangentWS : BITANGENTWS;
};

struct HS_PATCH_OUTPUT
{
	float Edges[3]	: SV_TessFactor;
	float Inside	: SV_InsideTessFactor;
};

HS_PATCH_OUTPUT hspatch(InputPatch<VS_OUTPUT, 3> ip, uint PatchID : SV_PrimitiveID)
{
	HS_PATCH_OUTPUT output;
	const float factor = 16.0f;

	output.Edges[0] = factor;
	output.Edges[1] = factor;
	output.Edges[2] = factor;

	output.Inside = factor;

	return output;
}

/*-------------------------------------------------------------------*/

[domain("tri")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("hspatch")]

HS_POINT_OUTPUT hs_main(InputPatch<VS_OUTPUT, 3> ip, uint i : SV_OutputControlPointID, uint PatchID : SV_PrimitiveID)
{
	HS_POINT_OUTPUT output;

	output.outPositionWS = ip[i].inPositionWS;
	output.outTexCoord = ip[i].inTexCoord;
	output.outNormalWS = ip[i].inNormalWS;
    output.outTangentWS = ip[i].inTangentWS;
    output.outBiTangentWS = ip[i].inBiTangentWS;

	return output;
}