cbuffer constantBuffer : register(b0)
{
    float4x4 orthographicProjectionMatrix;
}

float4 vs_deferred(in float3 inPosition : POSITION) : SV_POSITION
{
    /*Only data to forward to deferred pixelshader is clip space position, remaining data will be sampled
    from saved render target textures.*/
    float4 outPositionCS = mul(float4(inPosition, 1.0f), orthographicProjectionMatrix);
    
    return outPositionCS;
}