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
	//사각형
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
		//마우스 좌표 넘겨준후
		//한칸의 좌표가 마우스 위치 - terrainbrush의 크기보다 크면
	
	}
};
//Todo
//Buffer Class
//PerFreme Class
//Transform Class
//Renderer class
//주석 달기

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