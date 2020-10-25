static const float4 positions[4] =
{
    float4(-1, 1, 0, 0),
    float4(1, 1, 0, 0),
    float4(-1, -1, 0, 0),
    float4(1, -1, 0, 0),
};
static const float2 texcoords[4] =
{
    float2(0, 1),
    float2(1, 1),
    float2(0, 0),
    float2(1, 0),
};

cbuffer Matrices : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
    matrix lightWVP;
    float3 cameraPos;
};

struct GS_INPUT
{
    float3 position : Position;
};

struct PS_INPUT
{
    float4 position     : SV_Position;
    float2 texcoord     : texcoord;
    float3 normal       : normal;
    float3 positionws   : Position;
};

[maxvertexcount(4)]
void GSMAIN(point GS_INPUT input[1], inout TriangleStream<PS_INPUT> SpriteStream)
{
    PS_INPUT output;
    
    output.positionws = float4(input[0].position.xyz, 1);
    
    
    //Transform to view space
    float4 viewposition = float4(input[0].position.xyz, 1);
    viewposition = mul(viewposition, view);
    
    //Emit two new triangles
    for (int i = 0; i < 4; i++)
    {
        output.position = viewposition + positions[i];
        output.position = mul(output.position, projection);
        output.texcoord = texcoords[i];
        output.normal = float3(0.0f, 0.0f, 1.0f);
        SpriteStream.Append(output);
    }
    
    SpriteStream.RestartStrip();
}