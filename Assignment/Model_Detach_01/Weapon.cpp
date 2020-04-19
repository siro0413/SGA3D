#include "stdafx.h"
#include "Weapon.h"

Weapon::Weapon(Shader* shader, wstring file)
{
	weapon = new ModelRender(shader);

	weapon->ReadMaterial(L"Weapon/" + file);
	weapon->ReadMesh(L"Weapon/" + file);

	cTrans = new Transform();

	cTrans->Position(Vector3(Math::Random(-30.0f, 30.0f), 2, Math::Random(-30.0f, 30.0f)));
	cTrans->Scale(0.02f, 0.02f, 0.02f);
	Vector3 scale;
	cTrans->Scale(&scale);

	Vector3 pos;
	cTrans->Position(&pos);
	weapon->GetTransform()->Position(pos);
	weapon->GetTransform()->Scale(scale);

	cTrans->Scale(1.0f, 1.0f, 1.0f);

	coll = new Collider(cTrans);

}

Weapon::~Weapon()
{
	SafeDelete(coll);
	SafeDelete(cTrans);
	SafeDelete(weapon);
}

void Weapon::Update()
{
	weapon->Update();
	coll->Update();
}

void Weapon::Render()
{
	weapon->Render();
	weapon->Pass(1);
	coll->Render(bCheck ? Color(1, 0, 0, 1) : Color(0, 1, 0, 1));
}
