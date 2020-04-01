#include "Framework.h"
#include "Terrain.h"

Terrain::Terrain(Shader * shader, wstring heightFile)
	: Renderer(shader)
{
	sBaseMap = shader->AsSRV("BaseMap");

	heightMap = new Texture(heightFile);

	brushBuffer = new ConstantBuffer(&brushDesc, sizeof(BushDesc));
	sBrushBuffer = shader->AsConstantBuffer("CB_Brush"); // �޾ƾ��� �����̸�

	CreateVertexData();
	CreateIndexData();
	CreateNormalData();

	vertexBuffer = new VertexBuffer(vertices, vertexCount, sizeof(VertexTextureNormal), 0, false, true);
	//����Ʈ ���� UpdateSubresurse
	indexBuffer = new IndexBuffer(indices, indexCount);

}

Terrain::~Terrain()
{
	SafeDelete(heightMap);

	SafeDeleteArray(vertices);
	SafeDelete(vertexBuffer);

	SafeDeleteArray(indices);
	SafeDelete(indexBuffer);

	SafeDelete(baseMap);

}

void Terrain::Update()
{	
	Super::Update();
#pragma region boolCheck
	if (Rect)
	{
		Sphere = false;
		brushDesc.Type = 1;
	}
	else if (Sphere)
	{
		Rect = false;
		brushDesc.Type = 2;
	}
	else
		brushDesc.Type = 0;
#pragma endregion
	
	//ImGui::InputInt("Type", (int*)&brushDesc.Type);
	ImGui::Checkbox("Rect", &Rect);
	ImGui::SameLine();
	ImGui::Checkbox("Sphere", &Sphere);

	brushDesc.Type %= 3;

	ImGui::InputInt("Range", (int*)&brushDesc.Range);
	brushDesc.Range %= 20;

	if (brushDesc.Type > 0)
	{
		brushDesc.Location = GetPickedPosition();

		if (Mouse::Get()->Press(0))
			RaiseHeight(brushDesc.Location, brushDesc.Type, brushDesc.Range);
	}
}

void Terrain::Render()
{
	Super::Render();

	if(baseMap != NULL)
		sBaseMap->SetResource(baseMap->SRV());

	brushBuffer->Apply();
	sBrushBuffer->SetConstantBuffer(brushBuffer->Buffer());

	shader->DrawIndexed(0, Pass(), indexCount);
}

void Terrain::BaseMap(wstring file)
{
	SafeDelete(baseMap);

	baseMap = new Texture(file);
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

	//����
	float ddx = (position.x - v[0].x) / 1.0f;
	float ddz = (position.z - v[0].z) / 1.0f;

	//y(����) ���ϱ�
	Vector3 result;

	if(ddx + ddz <= 1)
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

	if(D3DXIntersectTri(&p[0], &p[1], &p[2], &start, &direction, &u, &v, &distance) == TRUE)
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
	Vp->UnProjection(&n, mouse, transform->World() , V, P);
	//NDC�� ����� ��ǥ�踦 ��ȯ
	//2D�� 3D�� �纯ȯ
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
	//�ȼ� ���� �޾ƿ���
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
			//�� �׸����� ���ϴ� �ε���
			UINT index = width * z + x;
			UINT pixel = width * (height - 1 - z) + x;

			vertices[index].Position.x = (float)x;
			vertices[index].Position.y = (heights[pixel].r * 256.0f) / 10.0f;
			vertices[index].Position.z = (float)z;

			vertices[index].Uv.x = (float)x / (float)width;
			vertices[index].Uv.y = (float)(height - 1 -z) / (float)height;
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
	//��ֺ���(����) ���Ϸ��� �� �ﰢ���� ���� ��ġ�� �˾ƾ���
	// -> ���� ��ġ�� �˾ƾ� ������ ���� �� �ֱ� ����

	//�ε��� ���� / 3 -> �ﰢ�� ����
	for (UINT i = 0; i < indexCount / 3; i++)
	{
		//�ε��������� ��ȣ�� �� �ﰢ���� ���� ��ȣ�� ��ȯ
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
				if (Keyboard::Get()->Press(VK_SHIFT) == false)
					vertices[index].Position.y += 5.0f *Time::Delta();
				if (Keyboard::Get()->Press(VK_SHIFT) == true)
					vertices[index].Position.y -= 5.0f *Time::Delta();
			}
		}
	}
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
				float cos = acosf(dist / range);
				float frac = sinf(cos);
				if (dist <= range && Keyboard::Get()->Press(VK_SHIFT) == false)
					vertices[index].Position.y += 5.0f *Time::Delta() * frac;
				if (dist <= range && Keyboard::Get()->Press(VK_SHIFT) == true)
					vertices[index].Position.y -= 5.0f *Time::Delta() * frac;

			}
		}
	}

	CreateNormalData(); //�������

	D3D::GetDC()->UpdateSubresource
	(
		vertexBuffer->Buffer(), 0, 0, vertices, sizeof(TerrainVertex)*vertexCount, 0
	);

}

