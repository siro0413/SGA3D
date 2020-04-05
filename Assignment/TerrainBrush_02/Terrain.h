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

	float GetHeight(Vector3& position);
	float GetHeightPick(Vector3& position);
	Vector3 GetPickedPosition(); //Raycast

private:
	void CreateVertexData();
	void CreateIndexData();
	void CreateNormalData();
	void RaiseHeight(Vector3& pos,UINT type, UINT range);
	void SlopeHeight(Vector3& pos, Vector3& pos2);

private:	
	Texture* heightMap;

	UINT width = 128;
	UINT height = 128;

	bool Rect = false;
	bool Sphere = false;
	bool Flat = false;
	bool Noise = false;
	bool Smooth = false;
	bool Slope = false;

	float tempY1, tempY2;
	TerrainVertex* vertices;

	UINT* indices;

	Texture* baseMap = NULL;
	ID3DX11EffectShaderResourceVariable* sBaseMap;

	struct BushDesc
	{
		Color Color = D3DXCOLOR(0, 1, 0, 1);
		Vector3 Location;
		UINT Type = 0;
		UINT Range = 1;
		float Padding[3];
	}brushDesc, brushDesc2;

	ConstantBuffer* brushBuffer;
	ID3DX11EffectConstantBuffer* sBrushBuffer;
};