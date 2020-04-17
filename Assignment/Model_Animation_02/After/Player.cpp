#include "stdafx.h"
#include "Player.h"

PlayerModel::PlayerModel(Shader * shader)
{
	player = new ModelAnimator(shader);

	player->ReadMaterial(L"Kachujin/Mesh");
	player->ReadMesh(L"Kachujin/Mesh");

	player->ReadClip(L"Kachujin/Idle");
	player->ReadClip(L"Kachujin/Running");
	player->ReadClip(L"Kachujin/Jump");
	player->ReadClip(L"Kachujin/Hip_Hop_Dancing");

	player->GetTransform()->Position(0, 0, 0);
	player->GetTransform()->Scale(0.025f, 0.025f, 0.025f);

	Vector3 cPos;
	player->GetTransform()->Position(&cPos);

	cTrans = new Transform();
	cTrans->Position(cPos.x, cPos.y + 2.5f, cPos.z);
	cTrans->Scale(2.0f, 5.0f, 2.3f);

	coll = new Collider(cTrans);
}

PlayerModel::~PlayerModel()
{
	SafeDelete(coll);
	SafeDelete(cTrans);
	SafeDelete(player);
}

void PlayerModel::Update()
{
	player->Update();
	coll->Update();
	Move();
}

void PlayerModel::Render()
{
	player->Render();
	player->Pass(2);
	coll->Render(bCheck ? Color(1, 0, 0, 1) : Color(0, 1, 0, 1));
}

void PlayerModel::Move()
{
	Vector3 cPos;
	player->GetTransform()->Position(&cPos);
	if (Keyboard::Get()->Press(VK_UP))
	{
		player->PlayClip(1, 1.0f, 0.7f);

		cPos.z += 5 * Time::Delta();
		player->GetTransform()->RotationDegree(0, 180, 0);


		player->GetTransform()->Position(cPos);
		cTrans->Position(cPos.x, cPos.y + 2.5f, cPos.z);
	}
	else if (Keyboard::Get()->Press(VK_DOWN))
	{
		player->PlayClip(1, 1, 0.7f);
		cPos.z -= 5 * Time::Delta();

		player->GetTransform()->RotationDegree(0, 0, 0);

		player->GetTransform()->Position(cPos);
		cTrans->Position(cPos.x, cPos.y + 2.5f, cPos.z);
	}

	if (Keyboard::Get()->Press(VK_RIGHT))
	{
		player->PlayClip(1, 1.0f, 0.7f);

		cPos.x += 5 * Time::Delta();
		player->GetTransform()->RotationDegree(0, -90, 0);


		player->GetTransform()->Position(cPos);
		cTrans->Position(cPos.x, cPos.y + 2.5f, cPos.z);
	}
	else if (Keyboard::Get()->Press(VK_LEFT))
	{
		player->PlayClip(1, 1, 0.7f);
		cPos.x -= 5 * Time::Delta();

		player->GetTransform()->RotationDegree(0, +90, 0);

		player->GetTransform()->Position(cPos);
		cTrans->Position(cPos.x, cPos.y + 2.5f, cPos.z);
	}


	if (Keyboard::Get()->Press(VK_UP) && (Keyboard::Get()->Press(VK_RIGHT)))
		player->GetTransform()->RotationDegree(0, -135, 0);

	else if (Keyboard::Get()->Press(VK_UP) && (Keyboard::Get()->Press(VK_LEFT)))
		player->GetTransform()->RotationDegree(0, 135, 0);

	if (Keyboard::Get()->Press(VK_DOWN) && (Keyboard::Get()->Press(VK_RIGHT)))
		player->GetTransform()->RotationDegree(0, -45, 0);

	else if (Keyboard::Get()->Press(VK_DOWN) && (Keyboard::Get()->Press(VK_LEFT)))
		player->GetTransform()->RotationDegree(0, 45, 0);

}
