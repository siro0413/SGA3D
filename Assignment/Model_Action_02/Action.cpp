#include "stdafx.h"
#include "ShadowDemo.h"
#include "Viewer/Freedom.h"
#include "Environment/Terrain.h"
#include "Environment/SkyCube.h"
#include "Objects/Projector.h"


void ShadowDemo::Initialize()
{
	srand((UINT)time(NULL));
	Performace perfomance;
	perfomance.Start();

	Context::Get()->GetCamera()->RotationDegree(23, 0, 0);
	Context::Get()->GetCamera()->Position(0, 57, -117);
	((Freedom *)Context::Get()->GetCamera())->Speed(20, 2);


	shader = new Shader(L"43_Shadow.fxo");
	sky = new SkyCube(L"Environment/GrassCube1024.dds");


	shadow = new Shadow(shader, Vector3(0, 0, 0), 65);

	shadow2D = new Render2D();
	shadow2D->GetTransform()->Position(150, D3D::Height() - 150, 0);
	shadow2D->GetTransform()->Scale(300, 300, 1);

	Mesh();
	Kachujin();
	Ganfaul();

}

void ShadowDemo::Destroy()
{
	SafeDelete(shader);
	SafeDelete(shadow);
	SafeDelete(shadow2D);
}

void ShadowDemo::Update()
{
	sky->Update();
	grid->Update();

	Move();

	kachujin->Update();
	auto attach = kachujin->GetAttachTransforms(0); //인스턴싱 번호
	collider[0].Collider->GetTransform()->World(attach);
	collider[0].Collider->Update();

	ganful->Update();
	MonsterMove(ganful);

}

void ShadowDemo::PreRender()
{
	shadow->Set();

	Pass(0, 1, 2);

	floor->Render();
	grid->Render();

	kachujin->Render();
	ganful->Render();
}

void ShadowDemo::Render()
{
	sky->Render();

	Pass(3, 4, 5);

	floor->Render();
	grid->Render();

	kachujin->Render();
	collider[0].Collider->Render(Color(0, 1, 0, 1));
	shadow2D->Update();
	shadow2D->SRV(shadow->SRV());
	shadow2D->Render();

	ganful->Render();
}

void ShadowDemo::Mesh()
{
	//Create Material
	{
		floor = new Material(shader);
		floor->DiffuseMap("Floor.png");
		floor->SpecularMap("SpecularMap.png");
		floor->NomalMap("Floor_Normal.png");
		floor->Specular(1, 1, 1, 20);
	}

	//Create Mesh
	{
		Transform* transform = NULL;

		grid = new MeshRender(shader, new MeshGrid(10, 10));
		transform = grid->AddTransform();
		transform->Position(0, 0, 0);
		transform->Scale(30, 1, 30);

	}
	grid->UpdateTransforms();

	meshs.push_back(grid);
}

void ShadowDemo::Kachujin()
{
	weapon = new Model();
	weapon->ReadMaterial(L"Weapon/Sword");
	weapon->ReadMesh(L"Weapon/Sword");

	kachujin = new ModelAnimator(shader);
	kachujin->ReadMaterial(L"Kachujin/Mesh");
	kachujin->ReadMesh(L"Kachujin/Mesh");

	kachujin->ReadClip(L"Kachujin/Idle");
	kachujin->ReadClip(L"Kachujin/Running");
	kachujin->ReadClip(L"Kachujin/Jump");
	kachujin->ReadClip(L"Kachujin/Hip_Hop_Dancing");

	Transform attachTransform;

	attachTransform.Position(-10, 0, -10);
	attachTransform.Scale(0.5f, 0.5f, 0.5f);

	kachujin->GetModel()->Attach(shader, weapon, 35, &attachTransform);

	Transform* transform = NULL;

	transform = kachujin->AddTransform();
	transform->Position(-25, 0, -30);
	transform->Scale(0.075f, 0.075f, 0.075f);
	kachujin->PlayClip(0, 0, 1.0f);

	kachujin->UpdateTransforms();

	animators.push_back(kachujin);


	collider[0].Init = new Transform();
	collider[0].Init->Scale(2.5f, 5.5f, 50);
	collider[0].Init->Position(-10, 0, -40);
	collider[0].Transform = new Transform();
	collider[0].Collider = new Collider(collider[0].Transform, collider[0].Init);

}

void ShadowDemo::Ganfaul()
{
	ganful = new ModelAnimator(shader);

	ganful->ReadMaterial(L"ganfaul/Mesh");
	ganful->ReadMesh(L"ganfaul/Mesh");

	ganful->ReadClip(L"ganfaul/ganfaul_Idle");
	ganful->ReadClip(L"ganfaul/ganfaul_Walking"); // walking
	ganful->ReadClip(L"ganfaul/ganfaul_Standing Melee Kick"); //공격
	ganful->ReadClip(L"ganfaul/ganfaul_Head Hit");//피격 

	Transform* trans = NULL;
	trans = ganful->AddTransform();
	trans->Position(-40, 0, 20);
	trans->Scale(0.075f, 0.075f, 0.075f);
	ganful->PlayClip(0, 0, 1.0f);
	ganful->UpdateTransforms();

	animators.push_back(ganful);
}

void ShadowDemo::Move()
{
	Vector3 pPos;
	kachujin->GetTransform(0)->Position(&pPos);

	if (Keyboard::Get()->Press(VK_UP))
	{
		kachujin->PlayClip(0, 1, 0.7f);

		pPos.z += 15 * Time::Delta();
		kachujin->GetTransform(0)->RotationDegree(0, 180, 0);


		kachujin->GetTransform(0)->Position(pPos);
		kachujin->UpdateTransforms();
		//cTrans->Position(pPos.x, pPos.y + 2.5f, pPos.z);
	}
	else if (Keyboard::Get()->Press(VK_DOWN))
	{
		kachujin->PlayClip(0, 1, 0.7f);
		pPos.z -= 15 * Time::Delta();

		kachujin->GetTransform(0)->RotationDegree(0, 0, 0);

		kachujin->GetTransform(0)->Position(pPos);
		kachujin->UpdateTransforms();
		//cTrans->Position(pPos.x, pPos.y + 2.5f, pPos.z);
	}

	if (Keyboard::Get()->Press(VK_RIGHT))
	{
		kachujin->PlayClip(0, 1, 0.7f);

		pPos.x += 15 * Time::Delta();
		kachujin->GetTransform(0)->RotationDegree(0, -90, 0);


		kachujin->GetTransform(0)->Position(pPos);
		kachujin->UpdateTransforms();
		//cTrans->Position(pPos.x, pPos.y + 2.5f, pPos.z);
	}
	else if (Keyboard::Get()->Press(VK_LEFT))
	{
		kachujin->PlayClip(0, 1, 0.7f);
		pPos.x -= 15 * Time::Delta();

		kachujin->GetTransform(0)->RotationDegree(0, +90, 0);

		kachujin->GetTransform(0)->Position(pPos);
		kachujin->UpdateTransforms();
		//cTrans->Position(pPos.x, pPos.y + 2.5f, pPos.z);
	}


	if (Keyboard::Get()->Press(VK_UP) && (Keyboard::Get()->Press(VK_RIGHT)))
	{
		kachujin->GetTransform(0)->RotationDegree(0, -135, 0);
		kachujin->UpdateTransforms();
	}

	else if (Keyboard::Get()->Press(VK_UP) && (Keyboard::Get()->Press(VK_LEFT)))
	{
		kachujin->GetTransform(0)->RotationDegree(0, 135, 0);
		kachujin->UpdateTransforms();
	}
	if (Keyboard::Get()->Press(VK_DOWN) && (Keyboard::Get()->Press(VK_RIGHT)))
	{
		kachujin->GetTransform(0)->RotationDegree(0, -45, 0);
		kachujin->UpdateTransforms();
	}
	else if (Keyboard::Get()->Press(VK_DOWN) && (Keyboard::Get()->Press(VK_LEFT)))
	{
		kachujin->GetTransform(0)->RotationDegree(0, 45, 0);
		kachujin->UpdateTransforms();
	}

	if (!Keyboard::Get()->Press(VK_UP) && !Keyboard::Get()->Press(VK_DOWN) && !Keyboard::Get()->Press(VK_LEFT) && !Keyboard::Get()->Press(VK_RIGHT))
	{
		kachujin->PlayClip(0, 0, 1.0f);
		kachujin->UpdateTransforms();
	}
}

void ShadowDemo::MonsterMove(ModelAnimator* model)
{
	static int count = 0;
	model->GetTransform(0)->Position(&gPos2);
	if (count == 0)
	{
		static float time = 0;
		time += Time::Delta();

		if (time >= 7.0f)
		{
			time = 0;
			count = 1;

			gPos = { Math::Random(-40.0f, 40.0f), 0.0f, Math::Random(-40.0f, 40.0f) };
			if (gPos.z - gPos2.z >= 0)
				model->GetTransform(0)->RotationDegree(0, 180, 0); //Up
			else if (gPos.z - gPos2.z <= 0)
				model->GetTransform(0)->RotationDegree(0, 0, 0);		//Down

			if (gPos.x - gPos2.x >= 0)
				model->GetTransform(0)->RotationDegree(0, -90, 0); //Left
			else if (gPos.x - gPos2.x <= 0)
				model->GetTransform(0)->RotationDegree(0, 90, 0); //Right

			if (gPos.x - gPos2.x >= 0 && gPos.z - gPos2.z >= 0) //UP LEFT
				model->GetTransform(0)->RotationDegree(0, -135, 0);
			else if (gPos.x - gPos2.x >= 0 && gPos.z - gPos2.z <= 0) //UP Right
				model->GetTransform(0)->RotationDegree(0, -45, 0);

			if (gPos.x - gPos2.x <= 0 && gPos.z - gPos2.z >= 0)
				model->GetTransform(0)->RotationDegree(0, 135, 0);
			else if (gPos.x - gPos2.x <= 0 && gPos.z - gPos2.z <= 0)
				model->GetTransform(0)->RotationDegree(0, 45, 0);

		}
	}
	if (count == 1)
	{
		
		model->PlayClip(0, 1, 1.0f);
		D3DXVec3Lerp(&gPos2, &gPos2, &gPos, 0.5f * Time::Delta());
		model->GetTransform(0)->Position(gPos2);
		model->UpdateTransforms();

		if (((int)gPos2.x == (int)gPos.x) && ((int)gPos2.z == (int)gPos.z))
		{
			count = 0;
			model->PlayClip(0, 0, 0.7f);
		}
	}

}

void ShadowDemo::Pass(UINT mesh, UINT model, UINT animation)
{
	for (auto* temp : meshs)
		temp->Pass(mesh);
	for (auto* temp : models)
		temp->Pass(model);
	for (auto* temp : animators)
		temp->Pass(animation);
}
