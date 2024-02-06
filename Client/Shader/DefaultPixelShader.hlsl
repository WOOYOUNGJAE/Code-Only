
struct PS_INPUT
{
    float4 projPosition : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

// Pixel Constant Buffer
sampler g_Sampler;

float4 main(PS_INPUT input) : SV_TARGET
{
    return tex2D(g_Sampler, input.texcoord);
}