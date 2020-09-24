SamplerState samplerState : SAMPLER : register(s0);
Texture2D textureObject : TEXTURE : register(t0);

cbuffer lightConstantBuffer : register(b0)
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 diffuseLightPosition;
	float diffuseLightIntensity;
	float ambientLightIntensity;
	float3 cameraPosition;
};

struct PS_INPUT
{
	float4 inPos : SV_POSITION;
	float2 inTexCoord : TEXCOORD;
	float3 inNormal : NORMAL;
	float3 posToPassToPS : POS;
};

float4 ps_main(PS_INPUT psInput) : SV_TARGET
{
	/*****TEXTURE*****/

	/*Texture color sampled from texture*/
	float4 textureColor = textureObject.Sample(samplerState, psInput.inTexCoord);
	
	/*****AMBIENT*****/

	/*Total ambient light affecting the scene vertices*/
	float4 ambientLight = ambientColor * ambientLightIntensity;

	/*****DIFFUSE*****/

	/*Total Diffuse light possibly affecting the vertices*/
	float4 diffuseLight = diffuseColor * diffuseLightIntensity;

	/*Creating unit vector pointing from actual vertex to diffuse light position*/
	float3 vecToLight = normalize(diffuseLightPosition - psInput.posToPassToPS);

	/*Calculating the diffuse scalar depending on the angle at which the light strikes the vertex*/
	float diffuseScalar = saturate(dot(vecToLight, normalize(psInput.inNormal)));

	/*Final diffuseColor given by the strength of the light and the light color (diffuseLight) multiplied by the diffuse scalar*/
	float4 finalDiffuseColor = diffuseLight * diffuseScalar;

	/*****SPECULAR*****/

	/*Normalized vector from light position to vertex position*/
	float3 lightToVec = normalize(psInput.posToPassToPS - diffuseLightPosition);

	/*The equivalent reflected vector, around the normal of the vertex position, and normalized*/
	float3 reflectLightToVec = normalize(reflect(lightToVec, normalize(psInput.inNormal)));

	/*Normalized vector from vector point to camera position*/
	float3 vecToCamera = normalize(cameraPosition - psInput.posToPassToPS);

	/*The specular scalar constant given by the angle between the two previous vectors, raised to some power.
	  The lower power, the greater area of specular light, and vice versa.*/
	float specularScalar = pow(saturate(dot(reflectLightToVec, vecToCamera)), 5);

	/*The final specular color, here based on white base color, and given by the multiplication with the specular scalae*/
	float3 finalSpecularColor = float3(1.0f, 1.0f, 1.0f) * specularScalar;

	/*The final pixel color, taking into account all light variables, texture color and angles of light hit*/
	float4 finalPixelColor = textureColor * (ambientLight + finalDiffuseColor + float4(finalSpecularColor, 1.0f));
    //return float4(0, 0, 1, 1);
	return finalPixelColor;
}