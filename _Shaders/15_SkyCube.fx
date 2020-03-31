#include "00_Global.fx"
#include "00_Light.fx"

float4 PS(MeshOutput input) : SV_Target0
{
    return SkyCubeMap.Sample(LinearSampler, input.oPosition);
}

technique11 T0
{
    P_VP(P0, VS_Mesh, PS) //º¯°æ
    P_RS_DSS_VP(P1, FrontCounterClockwise_True, DepthEnable_False, VS_Mesh, PS)
    

}