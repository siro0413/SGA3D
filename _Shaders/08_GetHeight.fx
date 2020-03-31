matrix World;
matrix View;
matrix Projection;

float3 LightDirection = float3(-1, -1, 1);
Texture2D BaseMap;

struct VertexInput
{
	float4 Position : Position0;
	float2 Uv : Uv;
	float3 Normal : Normal;	
};

struct VertexOutput
{
	float4 Position : SV_Position;
	float2 Uv : Uv;
	float3 Normal : Normal;
};

VertexOutput VS(VertexInput input)
{
	VertexOutput output;

	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);

	output.Normal = mul(input.Normal, (float3x3)World);
	output.Uv = input.Uv;

	return output;
}

float4 PS(VertexOutput input) : SV_Target0
{
    float3 diffuse = BaseMap.Sample(LinearSampler, input.Uv);
	float NdotL = dot(normalize(input.Normal), -LightDirection);

	return float4(diffuse * NdotL, 1);
}

RasterizerState RS
{
	FillMode = WireFrame;
};

technique11 T0
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}

	pass P1
	{
		SetRasterizerState(RS);

		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}

}