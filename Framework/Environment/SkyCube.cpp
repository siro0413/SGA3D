#include "Framework.h"
#include "SkyCube.h"

SkyCube::SkyCube(wstring file)
{
	shader = new Shader(L"15_SkyCube.fx");
	sphere = new MeshSphere(shader, 0.5f);

	wstring temp = L"../../_Textures/" + file;
	Check(D3DX11CreateShaderResourceViewFromFile
	(
		D3D::GetDevice(), temp.c_str(), NULL, NULL, &srv, NULL
	));

	sSrv = shader->AsSRV("SkyCubeMap");
}

SkyCube::~SkyCube()
{
	SafeDelete(shader);
	SafeDelete(sphere);
	SafeRelease(srv);
}

void SkyCube::Update()
{
	Vector3 pos;
	Context::Get()->GetCamera()->Position(&pos);

	sphere->GetTranform()->Position(pos);
}

void SkyCube::Render()
{
	sSrv->SetResource(srv);
	sphere->Pass(1);
	sphere->Render();
}

