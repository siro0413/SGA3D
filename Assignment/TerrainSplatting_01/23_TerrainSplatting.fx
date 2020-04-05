#include "00_Global.fx"
#include "00_Light.fx" 
#include "00_Terrain.fx"

float4 PS(VertexTerrain input) : SV_Target0
{

    float3 diffuse = GetLayerColor(input.Uv);
    float NdotL = dot(normalize(input.Normal), -GlobalLight.Direction);

    //float4 brush = input.Color; // VS
    float4 brush = GetBrushColor(input.wPosition); // VS
    float4 lineColor = GetLineColor(input.wPosition);
    
    float3 color = GetLayerColor(input.Uv);
    
    color += input.Color;
    color += input.BrushColor;
    
    return float4(color * NdotL, 1) + brush + lineColor;
}

RasterizerState RS
{
    FillMode = WireFrame;
};



technique11 T0
{
   P_VP(P0, VS, PS) //세미콜론 없음
   P_RS_VP(P1, RS, VS, PS) //<- 이건 RS 포함된거
}