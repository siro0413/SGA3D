#pragma once

class Terrain : public Renderer
{
public:
	typedef VertexTextureNormal TerrainVertex;

public:
	Terrain(Shader* shader, wstring heightFile);
	~Terrain();

	void Update();
	void Render();

	void BaseMap(wstring file);
	void LayerMap(wstring file, wstring alpha);

	float GetHeight(Vector3& position);
	float GetHeightPick(Vector3& position);
	Vector3 GetPickedPosition(); //Raycast

private:
	void CreateVertexData();
	void CreateIndexData();
	void CreateNormalData();
	void RaiseHeight(Vector3& pos,UINT type, UINT range);

	void BrushSplatting(Vector3&pos, UINT type, UINT range,UINT color);
private:	
	Texture* heightMap;

	UINT width = 128;
	UINT height = 128;

	TerrainVertex* vertices;

	UINT* indices;

	Texture* baseMap = NULL;
	ID3DX11EffectShaderResourceVariable* sBaseMap;

	Texture* layerMap = NULL;
	ID3DX11EffectShaderResourceVariable* sLayerMap;

	Texture* alphaMap = NULL;
	ID3DX11EffectShaderResourceVariable* sAlphaMap;

	bool bColor[4]
	{
		bColor[1] = false,
		bColor[2] = false,
		bColor[3] = false,
	};
	struct BushDesc
	{
		Color Color = D3DXCOLOR(0, 1, 0, 1);
		Vector3 Location;
		UINT Type = 0;
		UINT Range = 1;
		float Padding[3];
	}brushDesc;

	struct LineDesc
	{
		Color Color = D3DXCOLOR(1, 1, 1, 1);
		UINT Visible = 1;
		float Thickness = 0.01f;
		float Size = 5.0f;
		float Padding;
	}lineDesc;

	ConstantBuffer* brushBuffer;
	ID3DX11EffectConstantBuffer* sBrushBuffer;

	ConstantBuffer* lineBuffer;
	ID3DX11EffectConstantBuffer* sLineBuffer;
};