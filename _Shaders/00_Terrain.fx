Texture2D BaseMap;

struct VertexTerrain
{
	float4 Position : SV_Position0;
	float3 wPosition : Position1;
	float2 Uv : Uv0;
	float3 Normal : Normal0;
};

struct BrushDesc
{
	float4 Color;
	float3 Location;
	uint Type;
	uint Range;
};

cbuffer CB_Brush
{
	BrushDesc TerrainBrush;
};

float4 GetBrushColor(float3 wPosition)
{
	//�簢��
	[flatten]
	if (TerrainBrush.Type == 1)
	{
		[flatten]
		if ((wPosition.x >= ((TerrainBrush.Location.x - TerrainBrush.Range)) &&
			(wPosition.x <= ((TerrainBrush.Location.x + TerrainBrush.Range)) &&
			(wPosition.z >= ((TerrainBrush.Location.z - TerrainBrush.Range)) &&
			(wPosition.z <= ((TerrainBrush.Location.z + TerrainBrush.Range)))))))
		{
			return TerrainBrush.Color;
		}
		//���콺 ��ǥ �Ѱ�����
		//��ĭ�� ��ǥ�� ���콺 ��ġ - terrainbrush�� ũ�⺸�� ũ��
	
	}
};
//Todo
//Buffer Class
//PerFreme Class
//Transform Class
//Renderer class
//�ּ� �ޱ�

VertexTerrain VS(VertexTextureNormalinput)
{
	VertexTerrain output;
	output.Position = mul(input.Position, World);
	output.wPosition = output.Position.xyz;
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);

	output.Normal = mul(input.Normal, (float3x3) World);
	output.Uv = input.Uv;

	return output;
}