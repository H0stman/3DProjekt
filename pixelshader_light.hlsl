Texture2D textureNormal             : register(t0);
Texture2D textureDiffuse            : register(t1);
Texture2D texturePosition           : register(t2);

cbuffer lightConstantBuffer : register(b0)
{
    float3 ambientColor;
    float padding1;
    float3 diffuseColor;
    float padding2;
    float3 lightPosition;
    float lightIntensity;
    float3 paddingLight;
    float ambientLightIntensity;
    float attenuationConstant;
    float attenuationLinear;
    float attenuationQuadratic;
    float specularIntensity;
};

cbuffer transform : register(b1)
{
    float4x4 world;
    float4x4 view;
    float4x4 projection;
    float3 cameraPos;
};

float4 ps_light(in float4 inPositionSS : SV_POSITION) : SV_TARGET
{
    int3 indices = int3(inPositionSS.xy, 0);
    
    /*Retrieve all data stored in 2D-Textures (G-buffer) based on screen position*/
    float3 inNormalWS = normalize(textureNormal.Load(indices).xyz);
    float3 diffuseTextureColor = textureDiffuse.Load(indices).xyz;
    float3 inPositionWS = texturePosition.Load(indices).xyz;
    
    /**LIGHTNING**/
    
    /***Ambient***/
    
    //Calculate ambient light:
    float3 finalAmbientColor = ambientColor * ambientLightIntensity;
    
    /***Diffuse***/
    
    //Calculate vector from surface to light
    float3 surfaceToLightVector = lightPosition - inPositionWS;
    
    //Calculate length of vector (distance to light):
    float distanceToLight = length(surfaceToLightVector);
    
    //Calculate direction to Light (normalized vector to light):
    float3 directionToLightVector = normalize(surfaceToLightVector);
    
    //Calculate the attenutation of the light, i.e. the light fall-off:
    float attenuation = 1.0f / (attenuationConstant + (attenuationLinear * distanceToLight) + (attenuationQuadratic * distanceToLight * distanceToLight));
    
    //Calculating the diffuse scalar depending on the angle at which the light strikes the vertex
    float diffuseScalar = dot(directionToLightVector, inNormalWS);
    
    /***Calculate final diffuse and specular if light reaches surface***/    
    if (diffuseScalar > 0.0f)
    {
        float3 finalDiffuseColor = diffuseColor * lightIntensity * attenuation * diffuseScalar;
        
        /*The equivalent reflected vector, around the normal of the vertex position, and normalized*/
        float3 reflectLightToVec = reflect(-directionToLightVector, inNormalWS);
    
        /*Normalized vector from vector point to camera position*/
        float3 surfaceToCamera = normalize(cameraPos - inPositionWS);
    
        /*The specular scalar constant given by the angle between the two previous vectors, raised to some power.
	    The lower the power is, the greater area of specular light, and vice versa.*/
        float specularScalar = pow(saturate(dot(reflectLightToVec, surfaceToCamera)), 30.0f);
    
        /*The final specular color, using the diffuse color as its own color, also taking into account both specular
        intensity as well as point light attenuation.*/
        float3 finalSpecularColor = (diffuseColor * lightIntensity) * specularIntensity * specularScalar * attenuation;
        
        return float4(saturate((finalAmbientColor + finalDiffuseColor + finalSpecularColor) * diffuseTextureColor /** finalShadowFactor*/), 1.0f);
    }
    else
    {
        return float4(saturate((finalAmbientColor) * diffuseTextureColor /** finalShadowFactor*/), 1.0f);
    }
}