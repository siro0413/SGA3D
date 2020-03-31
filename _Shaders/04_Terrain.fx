matrix World;
matrix View;
matrix Projection;


struct VertexInput
{
	float4 Position : Position0;
	float3 Color : Color0;
};

struct VertexOutput
{
	float4 Position : SV_Position;
	float3 Color : Color0;
};

float3 HeightColor(float y)
{
	float3 color = 0.0f; //swizzling ����(Shader ����)
	
	if (y > 20.0f)
		color = float3(1, 1, 1);
	else if(y  > 15.0f)
		color = float3(1, 0, 0);
	else if (y  > 10.0f)
		color = float3(0, 1, 0);
	else if (y  > 5.0f)
		color = float3(0, 0, 1);
	else if (y  > 0.0f)
		color = float3(0, 0, 0);

	return color;
}

VertexOutput VS(VertexInput input)
{
	VertexOutput output;
	output.Position = mul(input.Position, World);

	output.Color = HeightColor(output.Position.y);

	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);

	return output;
}

float4 PS(VertexOutput input) : SV_Target0
{
	return float4(input.Color, 1);
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