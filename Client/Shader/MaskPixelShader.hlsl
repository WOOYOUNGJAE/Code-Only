
struct PS_INPUT
{
    float4 projPosition : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

float g_ViewportSizeX;
float g_ViewportSizeY;
float g_Radius;

float4 main(PS_INPUT input) : SV_TARGET
{
    float dist = sqrt(pow(input.projPosition.x - (g_ViewportSizeX / 2), 2) + pow(input.projPosition.y - (g_ViewportSizeY / 2), 2));
    if (dist < g_Radius)
    {
        discard;
    }
    
    return float4(0.0, 0.0, 0.0, 1.0);
}