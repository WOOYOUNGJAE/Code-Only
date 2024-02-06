
struct VS_INPUT
{
    float3 position : POSITION;
    float2 texcoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 projPosition : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

// Vertex Constant Buffer
matrix worldMatrix;
matrix viewMatrix;
matrix projectionMatrix;

float g_Amount;

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
 
    float4 pos = float4(input.position, 1.0f);
    pos = mul(pos, worldMatrix);
    pos = mul(pos, viewMatrix);
    pos = mul(pos, projectionMatrix);
    
    output.projPosition = pos;

    output.texcoord = input.texcoord;
    output.texcoord += g_Amount;
    
    return output;
}