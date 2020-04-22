#include "stdafx.h"
#include "InstancingDemo.h"
#include "Viewer/Freedom.h"
#include "Environment/Terrain.h"
#include "Environment/SkyCube.h"
#include "Environment/Billboard.h"


void InstancingDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(11, 0, 0);
	Context::Get()->GetCamera()->Position(0, 0, -17);
	((Freedom*)Context::Get()->GetCamera())->Speed(100, 2);

	shader = new Shader(L"30_Instancing.fx");
	perFrame = new PerFrame(shader);
	sMaps = shader->AsSRV("Maps");


	AddTexture(L"Red.png");
	AddTexture(L"Blue.png");
	//AddTexture(L"Blue1.png"); //아놔.. 얘 blue처럼보이는 red네..
	AddTexture(L"Green.png");
	AddTexture(L"White.png");
	AddTexture(L"Black.png");

	for (size_t i = 0; i < 500; i++)
	{
		transforms[i] = new Transform();
		//Transform ()함수인자에 shader를 지우면 순수한 위치정보만 넘어감
		transforms[i]->Position(Math::RandomVec3(-30, 30));
		transforms[i]->Scale(Math::RandomVec3(1.0f, 2.5f));
		transforms[i]->RotationDegree(Math::RandomVec3(-180.0f, 180.0f));

		worlds[i] = transforms[i]->World();
		instanceBuffer = new VertexBuffer(worlds[0], 500, sizeof(Matrix), 1);
	}
	CreateMesh();

}

void InstancingDemo::Destroy()
{
	SafeDelete(shader);

	for (auto* tran : transforms)
		SafeDelete(tran);
	SafeDelete(perFrame);
	SafeDelete(vertexBuffer);
	SafeDelete(instanceBuffer);
	SafeDelete(indexBuffer);
}

void InstancingDemo::Update()
{
	perFrame->Update();

}

void InstancingDemo::Render()
{
	perFrame->Render();


	vertexBuffer->Render();
	indexBuffer->Render();
	instanceBuffer->Render();

	if (textureFiles.size() > 0 && textures == NULL)
		textures = new TextureArray(textureFiles);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	sMaps->SetResource(textures->SRV());

	shader->DrawIndexedInstanced(0, 0, indices.size(), 500);

}

void InstancingDemo::CreateMesh()
{

	float w, h, d;
	w = h = d = 0.5f;

	//Front
	vertices.push_back(Mesh::MeshVertex(-w, -h, -d, 0, 1, 0, 0, -1));
	vertices.push_back(Mesh::MeshVertex(-w, +h, -d, 0, 0, 0, 0, -1));
	vertices.push_back(Mesh::MeshVertex(+w, +h, -d, 1, 0, 0, 0, -1));
	vertices.push_back(Mesh::MeshVertex(+w, -h, -d, 1, 1, 0, 0, -1));

	//Back
	vertices.push_back(Mesh::MeshVertex(-w, -h, +d, 1, 1, 0, 0, 1));
	vertices.push_back(Mesh::MeshVertex(+w, -h, +d, 0, 1, 0, 0, 1));
	vertices.push_back(Mesh::MeshVertex(+w, +h, +d, 0, 0, 0, 0, 1));
	vertices.push_back(Mesh::MeshVertex(-w, +h, +d, 1, 0, 0, 0, 1));

	//Top
	vertices.push_back(Mesh::MeshVertex(-w, +h, -d, 0, 1, 0, 1, 0));
	vertices.push_back(Mesh::MeshVertex(-w, +h, +d, 0, 0, 0, 1, 0));
	vertices.push_back(Mesh::MeshVertex(+w, +h, +d, 1, 0, 0, 1, 0));
	vertices.push_back(Mesh::MeshVertex(+w, +h, -d, 1, 1, 0, 1, 0));

	//Bottom
	vertices.push_back(Mesh::MeshVertex(-w, -h, -d, 1, 1, 0, -1, 0));
	vertices.push_back(Mesh::MeshVertex(+w, -h, -d, 0, 1, 0, -1, 0));
	vertices.push_back(Mesh::MeshVertex(+w, -h, +d, 0, 0, 0, -1, 0));
	vertices.push_back(Mesh::MeshVertex(-w, -h, +d, 1, 0, 0, -1, 0));

	//Left
	vertices.push_back(Mesh::MeshVertex(-w, -h, +d, 0, 1, -1, 0, 0));
	vertices.push_back(Mesh::MeshVertex(-w, +h, +d, 0, 0, -1, 0, 0));
	vertices.push_back(Mesh::MeshVertex(-w, +h, -d, 1, 0, -1, 0, 0));
	vertices.push_back(Mesh::MeshVertex(-w, -h, -d, 1, 1, -1, 0, 0));

	//Right
	vertices.push_back(Mesh::MeshVertex(+w, -h, -d, 0, 1, 1, 0, 0));
	vertices.push_back(Mesh::MeshVertex(+w, +h, -d, 0, 0, 1, 0, 0));
	vertices.push_back(Mesh::MeshVertex(+w, +h, +d, 1, 0, 1, 0, 0));
	vertices.push_back(Mesh::MeshVertex(+w, -h, +d, 1, 1, 1, 0, 0));


	indices =
	{
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23
	};

	vertexBuffer = new VertexBuffer(&vertices[0], vertices.size(), sizeof(Mesh::MeshVertex));
	indexBuffer = new IndexBuffer(&indices[0], indices.size());
}

void InstancingDemo::AddTexture(wstring file)
{
	SafeDelete(textures);
	//기존 뭐가들었을지모르는 texture는 지우고
	textureFiles.push_back(file);
	//새로들어온 file을 넘겨줌
}
