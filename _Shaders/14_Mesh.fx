//matrix World; < 삭제
//matrix View;
//matrix Projection;

#include "00_Global.fx" // <인클루드
#include "00_Light.fx"

//float4 Color 삭제

// Input 은 삭제

struct VertexOutput
{
    float4 Position : SV_Position;
    float2 Uv : Uv; //< 추가
    float3 Normal : Normal;
};


VertexOutput VS(VertexTextureNormal input) //<텍스처 노멀로 변경
{

    VertexOutput output;
    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.Normal = mul(input.Normal, (float3x3) World);
    output.Uv = input.Uv; //< Uv 넘겨주기

    return output;
}

float4 PS(VertexOutput input) : SV_Target0
{
    float3 diffuse = DiffuseMap.Sample(LinearSampler, input.Uv).rgb; // <추가
    float NdotL = dot(normalize(input.Normal), -GlobalLight.Direction); // <- 조명을 글로벌로 변경

    return float4(diffuse * NdotL, 1);
}

RasterizerState RS
{
    FillMode = WireFrame;
};

technique11 T0
{
    P_VP(P0, VS, PS) //변경
    P_RS_VP(P1, RS, VS, PS)
}