struct Particle
{
    float3 position;
};

StructuredBuffer<Particle> Particles : register(t0);

struct VS_INPUT
{
    uint vertexid : SV_VertexID;
};

struct GS_INPUT
{
    float3 position : Position;
};

GS_INPUT VSMAIN(in VS_INPUT input)
{
    GS_INPUT output;
    output.position.xyz = Particles[input.vertexid].position;
    return output;
}