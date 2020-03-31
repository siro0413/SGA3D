#include "00_Global.fx"
#include "00_Light.fx" 
#include "00_Terrain.fx"

float4 PS(VertexTerrain input) : SV_Target0
{

    float3 diffuse = BaseMap.Sample(LinearSampler, input.Uv);
    float NdotL = dot(normalize(input.Normal), -GlobalLight.Direction);

    return float4(diffuse * NdotL, 1);
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