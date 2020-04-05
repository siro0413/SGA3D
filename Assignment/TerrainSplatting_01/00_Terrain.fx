Texture2D BaseMap;
Texture2D AlphaMap;
Texture2D LayerMap;

struct VertexTerrain
{
    float4 Position : SV_Position0;
    float3 wPosition : Position1;
    float2 Uv : Uv0;
    float3 Normal : Normal;
    float4 Color : Color0;
    float4 BrushColor : Color1;
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
        if ((wPosition.x >= (TerrainBrush.Location.x - TerrainBrush.Range)) &&
			(wPosition.x <= (TerrainBrush.Location.x + TerrainBrush.Range)) &&
			(wPosition.z >= (TerrainBrush.Location.z - TerrainBrush.Range)) &&
			(wPosition.z <= (TerrainBrush.Location.z + TerrainBrush.Range)))
        {
            return TerrainBrush.Color;
        }
		//마우스 좌표 넘겨준후
		//한칸의 좌표가 마우스 위치 - terrainbrush의 크기보다 크면
    }
    //원
    [flatten]
    if (TerrainBrush.Type == 2)
    {
        float dx = wPosition.x - TerrainBrush.Location.x;
        float dz = wPosition.z - TerrainBrush.Location.z;
        //range 만큼 x사이즈 
        float dist = sqrt(dx * dx + dz * dz);
        
        [flatten]
        if (dist <= (float) TerrainBrush.Range)
            return TerrainBrush.Color;
    }
    
    return float4(0, 0, 0, 0);
};

struct LineDesc
{
    float4 Color;
    uint Visible;
    float Thickness;
    float Size;
};

cbuffer CB_TerrainLine
{
    LineDesc TerrainLine;
};

float4 GetBasicLineColor(float3 wPosition)
{
    [flatten]
    if (TerrainLine.Visible < 1)
        return float4(0, 0, 0, 0);
    
    float2 grid = wPosition.xz / TerrainLine.Size;
    grid = frac(grid);
    //frac = 정수 날려버리고 소수점만 남김
    
    float thick = TerrainLine.Thickness / TerrainLine.Size;
    
    [flatten]
    if (grid.x < thick || grid.y < thick)
        return TerrainLine.Color;
    
    return float4(0, 0, 0, 0);
}

float4 GetLineColor(float3 wPosition)
{
    [flatten]
    if (TerrainLine.Visible < 1)
        return float4(0, 0, 0, 0);
    
    float2 grid = wPosition.xz / TerrainLine.Size;
    float2 range = abs(frac(grid) - 0.5f);
    
    float2 speed = fwidth(grid);
    
    float2 pixel = range / speed;
    //abs(ddx(x)) + abs(ddy(y));
    
    float thick = saturate(min(pixel.x, pixel.y) - TerrainLine.Thickness);
    
    return lerp(TerrainLine.Color, float4(0, 0, 0, 0), thick);
    
}

float4 GetBaseColor(float2 uv)
{
    return BaseMap.Sample(LinearSampler, uv);
}

float4 GetLayerColor(float2 uv)
{
    float4 base = GetBaseColor(uv);
    float alpha = AlphaMap.Sample(LinearSampler, uv).b;
    float4 layer = LayerMap.Sample(LinearSampler, uv);

    
    
    return lerp(base, layer, (1 - alpha));
}

VertexTerrain VS(VertexColorTextureNormal input)
{
    VertexTerrain output;
    
    
    output.Position = mul(input.Position, World);
    output.wPosition = output.Position.xyz;
    
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);
    
    output.Normal = mul(input.Normal, (float3x3) World);
    output.Uv = input.Uv;
    output.Color = input.Color;
    
    output.BrushColor = GetBrushColor(output.wPosition);
    
    return output;
}