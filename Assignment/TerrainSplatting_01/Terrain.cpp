#include "Framework.h"
#include "Terrain.h"

Terrain::Terrain(Shader * shader, wstring heightFile)
	: Renderer(shader)
{
	sBaseMap = shader->AsSRV("BaseMap");
	sAlphaMap = shader->AsSRV("AlphaMap");
	sLayerMap = shader->AsSRV("LayerMap");

	heightMap = new Texture(heightFile);

	brushBuffer = new ConstantBuffer(&brushDesc, sizeof(BushDesc));
	sBrushBuffer = shader->AsConstantBuffer("CB_Brush"); // 받아야할 변수이름

	lineBuffer = new ConstantBuffer(&lineDesc, sizeof(LineDesc));
	sLineBuffer = shader->AsConstantBuffer("CB_TerrainLine"); // 받아야할 변수이름
	
	CreateVertexData();
	CreateIndexData();
	CreateNormalData();

	vertexBuffer = new VertexBuffer(vertices, vertexCount, sizeof(VertexTextureNormal), 0, false, true);
	//디폴트 권한 UpdateSubresurse
	indexBuffer = new IndexBuffer(indices, indexCount);

}

Terrain::~Terrain()
{
	SafeDelete(heightMap);

	SafeDeleteArray(vertices);
	SafeDelete(vertexBuffer);

	SafeDeleteArray(indices);
	SafeDelete(indexBuffer);

	SafeDelete(brushBuffer);
	SafeDelete(lineBuffer);

	SafeDelete(baseMap);
	SafeDelete(alphaMap);
	SafeDelete(layerMap);

}

void Terrain::Update()
{
	Super::Update();

	ImGui::InputInt("Type", (int*)&brushDesc.Type);
	brushDesc.Type %= 3;

	ImGui::InputInt("Range", (int*)&brushDesc.Range);
	brushDesc.Range %= 20;
	int color = 0;
	ImGui::Checkbox("Red", &bColor[1]);
	ImGui::SameLine();
	ImGui::Checkbox("Green", &bColor[2]);
	ImGui::SameLine();
	ImGui::Checkbox("Blue", &bColor[3]);
	if (bColor[1])
	{
		color = 1;
		bColor[0] = false;
		bColor[2] = false;
		bColor[3] = false;
	}
	else if (bColor[2])
	{
		color = 2;
		bColor[0] = false;
		bColor[1] = false;
		bColor[3] = false;
	}
	else if (bColor[3])
	{
		color = 3;
		bColor[0] = false;
		bColor[1] = false;
		bColor[2] = false;
	}
	else
	{
		color = 0;
		bColor[1] = false;
		bColor[2] = false;
		bColor[3] = false;
	}


	if (brushDesc.Type > 0)
	{
		brushDesc.Location = GetPickedPosition();

		if (Mouse::Get()->Press(0))
			//RaiseHeight(brushDesc.Location, brushDesc.Type, brushDesc.Range);
			BrushSplatting(brushDesc.Location, brushDesc.Type, brushDesc.Range, color);
	}

	//Line
	ImGui::Separator();
	ImGui::ColorEdit3("Color", lineDesc.Color);

	ImGui::InputInt("Visible", (int*)&lineDesc.Visible);
	lineDesc.Visible %= 2;

	ImGui::InputFloat("Thickness", &lineDesc.Thickness, 0.001f);
	lineDesc.Thickness = Math::Clamp(lineDesc.Thickness, 0.01f, 0.9f);

	ImGui::InputFloat("Size", &lineDesc.Size, 1.0f);
}

void Terrain::Render()
{
	Super::Render();

	if (baseMap != NULL)
		sBaseMap->SetResource(baseMap->SRV());

	brushBuffer->Apply();
	sBrushBuffer->SetConstantBuffer(brushBuffer->Buffer());

	lineBuffer->Apply();
	sLineBuffer->SetConstantBuffer(lineBuffer->Buffer());

	if (alphaMap != NULL && layerMap !=NULL)
	{
		sAlphaMap->SetResource(alphaMap->SRV());
		sLayerMap->SetResource(layerMap->SRV());
	}

	shader->DrawIndexed(0, Pass(), indexCount);
}

void Terrain::BaseMap(wstring file)
{
	SafeDelete(baseMap);

	baseMap = new Texture(file);
}

void Terrain::LayerMap(wstring file, wstring alpha)
{
	SafeDelete(alphaMap);
	SafeDelete(layerMap);

	alphaMap = new Texture(alpha);
	layerMap = new Texture(file);
}

float Terrain::GetHeight(Vector3 & position)
{
	UINT x = (UINT)position.x;
	UINT z = (UINT)position.z;

	if (x < 0 || x > width) return FLT_MIN;
	if (z < 0 || z > height) return FLT_MIN;

	UINT index[4];
	index[0] = width * z + x;
	index[1] = width * (z + 1) + x;
	index[2] = width * z + (x + 1);
	index[3] = width * (z + 1) + (x + 1);

	Vector3 v[4];
	for (int i = 0; i < 4; i++)
		v[i] = vertices[index[i]].Position;

	//기울기
	float ddx = (position.x - v[0].x) / 1.0f;
	float ddz = (position.z - v[0].z) / 1.0f;

	//y(높이) 구하기
	Vector3 result;

	if (ddx + ddz <= 1)
		result = v[0] + (v[2] - v[0]) * ddx + (v[1] - v[0]) * ddz;
	else
	{
		ddx = 1 - ddx;
		ddz = 1 - ddz;
		result = v[3] + (v[1] - v[3]) * ddx + (v[2] - v[3]) * ddz;
	}

	return result.y;
}

float Terrain::GetHeightPick(Vector3 & position)
{
	UINT x = (UINT)position.x;
	UINT z = (UINT)position.z;

	if (x < 0 || x > width) return FLT_MIN;
	if (z < 0 || z > height) return FLT_MIN;

	UINT index[4];
	index[0] = width * z + x;
	index[1] = width * (z + 1) + x;
	index[2] = width * z + (x + 1);
	index[3] = width * (z + 1) + (x + 1);

	Vector3 p[4];
	for (int i = 0; i < 4; i++)
		p[i] = vertices[index[i]].Position;

	Vector3 start(position.x, 1000, position.z);
	Vector3 direction(0, -1, 0);

	float u, v, distance;
	Vector3 result(-1, FLT_MIN, -1);

	if (D3DXIntersectTri(&p[0], &p[1], &p[2], &start, &direction, &u, &v, &distance) == TRUE)
		result = p[0] + (p[1] - p[0]) * u + (p[2] - p[0]) * v;

	if (D3DXIntersectTri(&p[3], &p[1], &p[2], &start, &direction, &u, &v, &distance) == TRUE)
		result = p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;

	return result.y;
}

Vector3 Terrain::GetPickedPosition()
{
	Matrix V = Context::Get()->View();
	Matrix P = Context::Get()->Projection();
	Viewport* Vp = Context::Get()->GetViewport();

	Vector3 mouse = Mouse::Get()->GetPosition();

	Vector3 n, f;
	mouse.z = 0.0f;
	Vp->UnProjection(&n, mouse, transform->World(), V, P);
	//NDC에 가까운 좌표계를 변환
	//2D를 3D로 재변환
	mouse.z = 1.0f;
	Vp->UnProjection(&f, mouse, transform->World(), V, P);

	Vector3 start = n;
	Vector3 direction = f - n;

	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			UINT index[4];
			index[0] = width * z + x;
			index[1] = width * (z + 1) + x;
			index[2] = width * z + (x + 1);
			index[3] = width * (z + 1) + (x + 1);

			Vector3 p[4];
			for (int i = 0; i < 4; i++)
				p[i] = vertices[index[i]].Position;

			float u, v, distance;
			if (D3DXIntersectTri(&p[0], &p[1], &p[2], &start, &direction, &u, &v, &distance) == TRUE)
				return p[0] + (p[1] - p[0]) * u + (p[2] - p[0]) * v;

			if (D3DXIntersectTri(&p[3], &p[1], &p[2], &start, &direction, &u, &v, &distance) == TRUE)
				return p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;
		}//for
	}//for

	//failure
	return Vector3(-1, FLT_MIN, -1);
}

void Terrain::CreateVertexData()
{
	//픽셀 색상 받아오기
	vector<Color> heights;

	heightMap->ReadPixel(DXGI_FORMAT_R8G8B8A8_UNORM, &heights);

	width = heightMap->GetWidth();
	height = heightMap->GetHeight();

	vertexCount = width * height;
	vertices = new TerrainVertex[vertexCount];

	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			//각 그리드의 좌하단 인덱스
			UINT index = width * z + x;
			UINT pixel = width * (height - 1 - z) + x;

			vertices[index].Position.x = (float)x;
			vertices[index].Position.y = (heights[pixel].r * 256.0f) / 10.0f;
			vertices[index].Position.z = (float)z;

			vertices[index].Uv.x = (float)x / (float)width;
			vertices[index].Uv.y = (float)(height - 1 - z) / (float)height;
		}
	}
}

void Terrain::CreateIndexData()
{
	indexCount = (width - 1) * (height - 1) * 6;
	indices = new UINT[indexCount];

	UINT index = 0;
	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			indices[index + 0] = width * z + x;
			indices[index + 1] = width * (z + 1) + x;
			indices[index + 2] = width * z + (x + 1);
			indices[index + 3] = width * z + (x + 1);
			indices[index + 4] = width * (z + 1) + x;
			indices[index + 5] = width * (z + 1) + (x + 1);

			index += 6;
		}
	}
}

void Terrain::CreateNormalData()
{
	//노멀벡터(수직) 구하려면 각 삼각형의 정점 위치를 알아야함
	// -> 정점 위치를 알아야 외적을 구할 수 있기 때문

	//인덱스 개수 / 3 -> 삼각형 개수
	for (UINT i = 0; i < indexCount / 3; i++)
	{
		//인덱스버퍼의 번호를 각 삼각형의 정점 번호로 변환
		UINT index0 = indices[i * 3 + 0]; //0
		UINT index1 = indices[i * 3 + 1]; //1
		UINT index2 = indices[i * 3 + 2]; //2

		TerrainVertex v0 = vertices[index0];
		TerrainVertex v1 = vertices[index1];
		TerrainVertex v2 = vertices[index2];

		Vector3 d1 = v1.Position - v0.Position;
		Vector3 d2 = v2.Position - v0.Position;

		Vector3 normal;
		D3DXVec3Cross(&normal, &d1, &d2);

		vertices[index0].Normal = normal;
		vertices[index1].Normal = normal;
		vertices[index2].Normal = normal;
	}

	for (UINT i = 0; i < vertexCount; i++)
		D3DXVec3Normalize(&vertices[i].Normal, &vertices[i].Normal);
}

void Terrain::RaiseHeight(Vector3 & pos, UINT type, UINT range)
{
	D3D11_BOX rect;
	rect.left = (LONG)pos.x - range;
	rect.right = (LONG)pos.x + range;
	rect.bottom = (LONG)pos.z - range;
	rect.top = (LONG)pos.z + range;

	if (rect.left < 0) rect.left = 0;
	if (rect.right >= width) rect.right = width;
	if (rect.bottom < 0) rect.bottom = 0;
	if (rect.top >= height) rect.top = height;

	if (type == 1)
	{
		for (LONG z = rect.bottom; z < rect.top; z++)
		{
			for (LONG x = rect.left; x < rect.right; x++)
			{
				UINT index = width * (UINT)z + (UINT)x;
				vertices[index].Position.y += 5.0f *Time::Delta();
			}
		}
	}
	//CreateNormalData(); //조명관리

	else if (type == 2)
	{
		for (LONG z = rect.bottom; z < rect.top; z++)
		{
			for (LONG x = rect.left; x < rect.right; x++)
			{
				UINT index = width * z + x;

				float dx = pos.x - x;
				float dz = pos.z - z;
				float dist = sqrt(dx * dx + dz * dz);
				float angle = acosf(dist / range);
				float frac = sinf((dist/range));
				if (dist <= range)
					vertices[index].Position.y += 5.0f *Time::Delta() * frac;
			}
		}
	}



	CreateNormalData(); //조명관리

	D3D::GetDC()->UpdateSubresource
	(
		vertexBuffer->Buffer(), 0, 0, vertices, sizeof(TerrainVertex)*vertexCount, 0
	);

}

void Terrain::BrushSplatting(Vector3 & pos, UINT type, UINT range, UINT color)
{
	D3D11_BOX rect;
	rect.left = (LONG)pos.x - range;
	rect.right = (LONG)pos.x + range;
	rect.bottom = (LONG)pos.z - range;
	rect.top = (LONG)pos.z + range;

	if (rect.left < 0) rect.left = 0;
	if (rect.right >= width) rect.right = width;
	if (rect.bottom < 0) rect.bottom = 0;
	if (rect.top >= height) rect.top = height;


	if (type == 1)
	{
		for (LONG z = rect.bottom; z < rect.top; z++)
		{
			for (LONG x = rect.left; x < rect.right; x++)
			{
				UINT index = width * (UINT)z + (UINT)x;
				switch (color)
				{
				case 0: vertices[index].Color = D3DXCOLOR(0, 0, 0, 1); break;
				case 1: vertices[index].Color.r = 1; break;
				case 2: vertices[index].Color.g = 1; break;
				case 3: vertices[index].Color.b = 1; break;
				default:
					break;
				}
			}
		}
	}
	


	CreateNormalData(); //조명관리

	D3D::GetDC()->UpdateSubresource
	(
		vertexBuffer->Buffer(), 0, 0, vertices, sizeof(TerrainVertex)*vertexCount, 0
	);

}
