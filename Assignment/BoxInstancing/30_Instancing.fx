#include "00_Global.fx"
#include "00_Light.fx"

struct VertexOutput
{
    float4 Position : SV_Position;
    float3 Normal : Normal;
    float2 Uv : Uv0;

    uint InstanceID : InstaceID;
};

VertexOutput VS(VertexTextureInstance input)
{
    VertexOutput output;

    World = input.Transform;
    
    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    output.Normal = mul(input.Normal, (float3x3) World);
    output.Uv = input.Uv;
    output.InstanceID = input.InstanceID;
    return output;
}

Texture2DArray Maps;
float4 PS(VertexOutput input) : SV_Target0
{
    float4 diffuse = Maps.Sample(LinearSampler, float3(input.Uv, input.InstanceID % 5));
    //float3 diffuse = DiffuseMap.Sample(LinearSampler, input.Uv);

    //float NdotL = dot(normalize(input.Normal), -GlobalLight.Direction);

    return diffuse;
}

technique11 T0
{
    P_VP(P0, VS, PS)
}