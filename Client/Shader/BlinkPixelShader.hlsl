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
    
    //�ȼ��� ���İ��� 100 �̻��� �� 
    //discard
    //�ƴϸ� �ؿ� ����
    if (color.a < 0.39)
    {
        discard;
    }

    color = float4(color.x + g_Ratio, color.y - g_Ratio, color.z - g_Ratio, 1.0);
    //color�� 1,2,3��° ��Ҵ� 0���� 1���̿��� ��

    return color;
}