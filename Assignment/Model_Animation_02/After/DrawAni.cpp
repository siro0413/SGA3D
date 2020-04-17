#include "stdafx.h"
#include "DrawAni.h"
#include "Viewer/Freedom.h"
#include "Object/Enemy.h"
#include "Object/Player.h"

void DrawAni::Initialize()
{
	srand((UINT)time(NULL));

	Context::Get()->GetCamera()->RotationDegree(0, 0, 0);
	Context::Get()->GetCamera()->Position(0, 6, -45);
	//((*)Context::Get()->GetCamera())->Speed(20, 2);

	shader = new Shader(L"26_Animation.fx");
	for (size_t i = 0; i < 3; i++)
		enemy[i] = new EnemyModel(shader);

	player = new PlayerModel(shader);
}

void DrawAni::Destroy()
{
	SafeDelete(player);
	for (size_t i = 0; i < 3; i++)
		SafeDelete(enemy[i]);
}

void DrawAni::Update()
{
	player->Update();
	for (size_t i = 0; i < 3; i++)
		enemy[i]->Update();
}

void DrawAni::Render()
{
	player->Render();
	for (size_t i = 0; i < 3; i++)
	{
		enemy[i]->Render();
		enemy[i]->SetIntersect(player->PlayerCollider());
	}
	if (enemy[0]->GetBool() || enemy[1]->GetBool() || enemy[2]->GetBool())
		player->SetBool(true);
	else
		player->SetBool(false);
}

