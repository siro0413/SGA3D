#include "stdafx.h"
#include "Enemy.h"

EnemyModel::EnemyModel(Shader * shader)
{
	enemy = new ModelAnimator(shader);

	enemy->ReadMaterial(L"Kachujin/Mesh");
	enemy->ReadMesh(L"Kachujin/Mesh");

	enemy->ReadClip(L"Kachujin/Idle");
	enemy->ReadClip(L"Kachujin/Jump");
	enemy->ReadClip(L"Kachujin/Running");
	enemy->ReadClip(L"Kachujin/Hip_Hop_Dancing");

	rPos;
	rPos = { Math::Random(-40.0f, 20.0f), 0.0f, Math::Random(-40.0f, 20.0f) };

	cTrans = new Transform();
	cTrans->Position(rPos.x, rPos.y + 2.5f, rPos.z);
	cTrans->Scale(2.0f, 5.0f, 2.3f);
	
	enemy->GetTransform()->Position(rPos);
	enemy->GetTransform()->Scale(0.025f, 0.025f, 0.025f);

	coll = new Collider(cTrans);

	clip = 0;
}

EnemyModel::~EnemyModel()
{
	SafeDelete(coll);
	SafeDelete(cTrans);
	SafeDelete(enemy);

}

void EnemyModel::Update()
{

	AutoMove();
	enemy->Update();
	coll->Update();
}

void EnemyModel::Render()
{
	enemy->Render();
	enemy->Pass(2);
	coll->Render(bCheck ? Color(1,0,0,1) : Color(0,1,0,1));
}

void EnemyModel::AutoMove()
{

	enemy->GetTransform()->Position(&rPos2);
	if (bCheck)
	{
		clip = 1;
		enemy->PlayClip(clip, 1.0f, 0.7f);
	}

	if (clip == 1)
	{
		static float time = 0.0f;
		time += Time::Delta();
		if (time >= 1.0f)
		{
			clip = 2;
			time = 0;
			enemy->PlayClip(clip, 1.0f, 0.7f);
			rPos = { Math::Random(-40.0f, 20.0f), 0.0f, Math::Random(-40.0f, 20.0f) };


			if (rPos.z - rPos2.z >= 0)
				enemy->GetTransform()->RotationDegree(0, 180, 0); //Up
			else if (rPos.z - rPos2.z <= 0)
				enemy->GetTransform()->RotationDegree(0, 0, 0);		//Down

			if (rPos.x - rPos2.x >= 0)
				enemy->GetTransform()->RotationDegree(0, -90, 0); //Left
			else if (rPos.x - rPos2.x <= 0)
				enemy->GetTransform()->RotationDegree(0, 90, 0); //Right

			if (rPos.x - rPos2.x >= 0 && rPos.z - rPos2.z >= 0) //UP LEFT
				enemy->GetTransform()->RotationDegree(0, -135, 0);
			else if (rPos.x - rPos2.x >= 0 && rPos.z - rPos2.z <= 0) //UP Right
				enemy->GetTransform()->RotationDegree(0, -45, 0);

			if (rPos.x - rPos2.x <= 0 && rPos.z - rPos2.z >= 0)
				enemy->GetTransform()->RotationDegree(0, 135, 0);
			else if (rPos.x - rPos2.x <= 0 && rPos.z - rPos2.z <= 0)
				enemy->GetTransform()->RotationDegree(0, 45, 0);
		}

	}

	if (clip == 2)
	{
		enemy->PlayClip(clip, 1.0f, 0.7f);
	


		D3DXVec3Lerp(&rPos2, &rPos2, &rPos, 0.5f*Time::Delta());
		cTrans->Position(rPos2.x, rPos2.y + 2.5f, rPos2.z);
		enemy->GetTransform()->Position(rPos2);

		if (((int)rPos2.x == (int)rPos.x) && ((int)rPos2.z == (int)rPos.z))
		{
			clip = 0;
			enemy->PlayClip(clip, 1.0f, 0.7f);
		}
	}


}

