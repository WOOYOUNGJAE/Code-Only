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
    
    //픽셀의 알파값이 100 이상일 때 
    //discard
    //아니면 밑에 적용
    if (color.a < 0.39)
    {
        discard;
    }

    color = float4(color.x + g_Ratio, color.y - g_Ratio, color.z - g_Ratio, 1.0);
    //color의 1,2,3번째 요소는 0에서 1사이여야 함

    return color;
}