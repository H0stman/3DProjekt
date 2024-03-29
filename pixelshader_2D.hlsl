SamplerState samplerState : SAMPLER : register(s0);
Texture2D textureObject : TEXTURE : register(t0);

struct PS_INPUT
{
	float4 inPositionSS : SV_POSITION;
	float2 inTexCoord : TEXCOORD;
};

float4 ps_2D_main(in PS_INPUT psInput) : SV_TARGET
{
	/*****TEXTURE*****/

	/*Texture color sampled from texture*/
	float4 textureColor = textureObject.Sample(samplerState, psInput.inTexCoord);
	//float4 textureColor = float4(psInput.inTexCoord.x, psInput.inTexCoord.y, 0.0, 1.0);
	
	return textureColor;
}
