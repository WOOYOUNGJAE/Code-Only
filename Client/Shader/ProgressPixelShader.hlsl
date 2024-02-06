
struct PS_INPUT
{
    float4 projPosition : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

// Pixel Constant Buffer
sampler g_Sampler;

float g_Ratio; // 0 ~ 1

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 color = tex2D(g_Sampler, input.texcoord);
    if (input.texcoord.x > g_Ratio)
    {
        color = float4(1.0, 1.0, 1.0, 0.0);
    }
    
    return color;
}