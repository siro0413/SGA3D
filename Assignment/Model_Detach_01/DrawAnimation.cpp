#include "stdafx.h"
#include "DrawAnimation.h"
#include "Viewer/Freedom.h"
#include "Object/Weapon.h"


void DrawAnimation::Initialize()
{
	srand((UINT)time(NULL));

	Context::Get()->GetCamera()->RotationDegree(0, 0, 0);
	Context::Get()->GetCamera()->Position(0, 6, -45);
	//((*)Context::Get()->GetCamera())->Speed(20, 2);

	shader = new Shader(L"26_Animation.fx");

	vector<wstring> weapos;

	weapos.push_back(L"Cutter");
	weapos.push_back(L"Cutter2");
	weapos.push_back(L"Dagger_epic");
	weapos.push_back(L"Dagger_small");
	weapos.push_back(L"Katana");
	weapos.push_back(L"Rapier");
	weapos.push_back(L"Sword");
	weapos.push_back(L"Sword_epic");
	weapos.push_back(L"Sword2");

	for (size_t i = 0; i < weapos.size(); i++)
		we[i] = new Weapon(shader, weapos[i]);
	weapon = new Model[9];
	Weapon_();


	Kachujin();
}

void DrawAnimation::Destroy()
{
	SafeDelete(kachujin);
	for (size_t i = 0; i < 9; i++)
		SafeDelete(we[i]);

	SafeDelete(coll);
	SafeDelete(collTransform);
}

void DrawAnimation::Update()
{

	kachujin->GetTransform()->Position(&pos);
	//for (auto wea : weapons)
	//	wea->Update();
	for (size_t i = 0; i < 9; i++)
		we[i]->Update();

	if (kachujin != NULL)
	{
		if (Keyboard::Get()->Press(VK_UP))
		{
			kachujin->PlayClip(1, 1.0f, 0.7f);

			pos.z += 5 * Time::Delta();
			kachujin->GetTransform()->RotationDegree(0, 180, 0);


			kachujin->GetTransform()->Position(pos);
			collTransform->Position(pos.x, pos.y + 2.5f, pos.z);
		}
		else if (Keyboard::Get()->Press(VK_DOWN))
		{
			kachujin->PlayClip(1, 1, 0.7f);
			pos.z -= 5 * Time::Delta();

			kachujin->GetTransform()->RotationDegree(0, 0, 0);

			kachujin->GetTransform()->Position(pos);
			collTransform->Position(pos.x, pos.y + 2.5f, pos.z);
		}

		if (Keyboard::Get()->Press(VK_RIGHT))
		{
			kachujin->PlayClip(1, 1.0f, 0.7f);

			pos.x += 5 * Time::Delta();
			kachujin->GetTransform()->RotationDegree(0, -90, 0);


			kachujin->GetTransform()->Position(pos);
			collTransform->Position(pos.x, pos.y + 2.5f, pos.z);
		}
		else if (Keyboard::Get()->Press(VK_LEFT))
		{
			kachujin->PlayClip(1, 1, 0.7f);
			pos.x -= 5 * Time::Delta();

			kachujin->GetTransform()->RotationDegree(0, +90, 0);

			kachujin->GetTransform()->Position(pos);
			collTransform->Position(pos.x, pos.y + 2.5f, pos.z);
		}


		if (Keyboard::Get()->Press(VK_UP) && (Keyboard::Get()->Press(VK_RIGHT)))
			kachujin->GetTransform()->RotationDegree(0, -135, 0);

		else if (Keyboard::Get()->Press(VK_UP) && (Keyboard::Get()->Press(VK_LEFT)))
			kachujin->GetTransform()->RotationDegree(0, 135, 0);

		if (Keyboard::Get()->Press(VK_DOWN) && (Keyboard::Get()->Press(VK_RIGHT)))
			kachujin->GetTransform()->RotationDegree(0, -45, 0);

		else if (Keyboard::Get()->Press(VK_DOWN) && (Keyboard::Get()->Press(VK_LEFT)))
			kachujin->GetTransform()->RotationDegree(0, 45, 0);

		coll->Update();
		kachujin->Update();
	}





}

void DrawAnimation::Render()
{
	if (kachujin != NULL) kachujin->Render();

	coll->Render(Color(0, 1, 0, 1));
	for (size_t i = 0; i < 9; i++)
	{
		we[i]->Render();
		we[i]->SetIntersect(coll);
		Transform attachTransform;
		attachTransform.Position(-10, 0, -10);
		attachTransform.Scale(0.5f, 0.5f, 0.5f);
		bCheck = we[i]->GetBool();

		if (bCheck)
			kachujin->GetModel()->Attach(shader, &weapon[i], 35, &attachTransform);
	}
	
}

void DrawAnimation::Kachujin()
{
	kachujin = new ModelAnimator(shader);
	kachujin->ReadMaterial(L"Kachujin/Mesh");
	kachujin->ReadMesh(L"Kachujin/Mesh");
	kachujin->ReadClip(L"Kachujin/Idle");
	kachujin->ReadClip(L"Kachujin/Running");
	kachujin->ReadClip(L"Kachujin/Jump");
	kachujin->ReadClip(L"Kachujin/Hip_Hop_Dancing");
	kachujin->GetTransform()->Position(3, 0, -20);
	kachujin->GetTransform()->Scale(0.025f, 0.025f, 0.025f);

	kachujin->GetTransform()->Position(&pos);

	collTransform->Position(pos.x, pos.y + 2.5f, pos.z);
	collTransform->Scale(2, 5, 2.3f);

	

	coll = new Collider(collTransform);

	kachujin->Pass(2);

}

void DrawAnimation::Weapon_()
{
	vector<wstring> weapos;

	weapos.push_back(L"Weapon/Cutter");
	weapos.push_back(L"Weapon/Cutter2");
	weapos.push_back(L"Weapon/Dagger_epic");
	weapos.push_back(L"Weapon/Dagger_small");
	weapos.push_back(L"Weapon/Katana");
	weapos.push_back(L"Weapon/Rapier");
	weapos.push_back(L"Weapon/Sword");
	weapos.push_back(L"Weapon/Sword_epic");
	weapos.push_back(L"Weapon/Sword2");

	Transform attachTransform;
	attachTransform.Position(-5.0f, 0, 5.0f);
	attachTransform.Scale(0.5f, 0.5f, 0.5f);

	for (size_t i = 0; i < weapos.size() - 1; i++)
	{
		weapon[i].ReadMaterial(weapos[i]);
		weapon[i].ReadMesh(weapos[i]);
	}
}


