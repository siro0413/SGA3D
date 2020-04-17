#include "stdafx.h"
#include "DrawAnimation.h"
#include "Viewer/Freedom.h"


void DrawAnimation::Initialize()
{

	srand((UINT)time(NULL));
	Context::Get()->GetCamera()->RotationDegree(0, 0, 0);
	Context::Get()->GetCamera()->Position(0, 6, -45);
	//((*)Context::Get()->GetCamera())->Speed(20, 2);

	shader = new Shader(L"26_Animation.fx");

	Kachujin();
	Kachujin2();
	Kachujin3();
}

void DrawAnimation::Destroy()
{
	SafeDelete(kachujin);
	SafeDelete(kachujin2);
	SafeDelete(kachujin3);
	SafeDelete(weapon);

	SafeDelete(coll);
	SafeDelete(coll2);
	SafeDelete(coll3);

}

void DrawAnimation::Update()
{

	kachujin->GetTransform()->Position(&pos);

	kachujin2->GetTransform()->Position(&Mpos[0]);
	kachujin3->GetTransform()->Position(&Mpos[1]);
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

	if (kachujin2 != NULL)
	{
		coll2->Update();
		kachujin2->Update();
	}
	if (kachujin3 != NULL) 
	{

		if (bCheck2)
		{
			clip = 1;
			kachujin3->PlayClip(clip, 1.0f, 0.7f);
		}

		if (clip == 1)
		{
			static float time = 0.0f;
			time += Time::Delta();
			if(time >= 2.79f)
			{
				clip = 2;
				kachujin3->PlayClip(clip, 1.0f, 0.7f);
				temp = { Math::Random(-20.0f, 20.0f),0,Math::Random(-20.0f, 20.0f) };
				time = 0;
			}		

		}

		if (clip == 2)
		{
			kachujin3->PlayClip(clip, 1.0f, 0.7f);
			//temp = { Math::Random(-40.0f, 40.0f),0,Math::Random(-40.0f,40.0f) };
			D3DXVec3Lerp(&Mpos[1], &Mpos[1], &temp, 0.5f*Time::Delta());
			collTransform3->Position(Mpos[1].x, Mpos[1].y + 2.5f, Mpos[1].z);
			kachujin3->GetTransform()->Position(Mpos[1]);


			if (temp.z - Mpos[1].z >= 0)
				kachujin3->GetTransform()->RotationDegree(0, 180, 0); //Up
			else if (temp.z - Mpos[1].z <= 0)
				kachujin3->GetTransform()->RotationDegree(0, 0, 0);		//Down

			if (temp.x - Mpos[1].x >= 0)
				kachujin3->GetTransform()->RotationDegree(0, -90, 0); //Left
			else if (temp.x - Mpos[1].x <= 0)
				kachujin3->GetTransform()->RotationDegree(0, 90, 0); //Right

			if (temp.x - Mpos[1].x >= 0 && temp.z - Mpos[1].z >= 0) //UP LEFT
				kachujin3->GetTransform()->RotationDegree(0, -135, 0);
			else if (temp.x - Mpos[1].x >= 0 && temp.z - Mpos[1].z <= 0) //UP Right
				kachujin3->GetTransform()->RotationDegree(0, -45, 0);

			if (temp.x - Mpos[1].x <= 0 && temp.z - Mpos[1].z >= 0)
				kachujin3->GetTransform()->RotationDegree(0, 135, 0);
			else if (temp.x - Mpos[1].x <= 0 && temp.z - Mpos[1].z <= 0)
				kachujin3->GetTransform()->RotationDegree(0, 45, 0);



			if (((int)Mpos[1].x == (int)temp.x )&& ((int)Mpos[1].z == (int)temp.z))
			{
				clip = 0;
				kachujin3->PlayClip(clip, 1.0f, 0.7f);
			}
		}


		coll3->Update();
		kachujin3->Update();
	}

}

void DrawAnimation::Render()
{
	if (kachujin != NULL) kachujin->Render();
	if (kachujin2 != NULL) kachujin2->Render();
	if (kachujin3 != NULL) kachujin3->Render();

	bCheck = coll->IsIntersect(coll2);
	bCheck2 = coll->IsIntersect(coll3);
	coll->Render(bCheck || bCheck2 ? Color(1, 0, 0, 1) : Color(0, 1, 0, 1));
	coll2->Render(bCheck ? Color(1, 0, 0, 1) : Color(0, 1, 0, 1));
	coll3->Render(bCheck2 ? Color(1, 0, 0, 1) : Color(0, 1, 0, 1));
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

	weapon = new Model();
	weapon->ReadMaterial(L"Weapon/Sword");
	weapon->ReadMesh(L"Weapon/Sword");

	Transform attachTransform;
	attachTransform.Position(-10, 0, -10);
	attachTransform.Scale(0.5f, 0.5f, 0.5f);

	kachujin->GetModel()->Attach(shader, weapon, 35, &attachTransform);

	kachujin->GetTransform()->Position(&pos);

	collTransform->Position(pos.x, pos.y + 2.5f, pos.z);
	collTransform->Scale(2, 5, 2.3f);

	coll = new Collider(collTransform);

	kachujin->Pass(2);

}
void DrawAnimation::Kachujin2()
{
	kachujin2 = new ModelAnimator(shader);

	kachujin2->ReadMaterial(L"Kachujin/Mesh");
	kachujin2->ReadMesh(L"Kachujin/Mesh");
	kachujin2->ReadClip(L"Kachujin/Idle");

	temp = { Math::Random(-20.0f, 20.0f),0,Math::Random(-20.0f,20.0f) };

	kachujin2->GetTransform()->Position(0, 0, 0);
	//kachujin2->GetTransform()->Position(temp.x, 0, temp.z);
	kachujin2->GetTransform()->Scale(0.025f, 0.025f, 0.025f);

	kachujin2->GetTransform()->Position(&Mpos[0]);

	collTransform2->Position(Mpos[0].x, Mpos[0].y + 2.5f, Mpos[0].z);
	collTransform2->Scale(2, 5, 2.3f);

	coll2 = new Collider(collTransform2);

	kachujin2->Pass(2);
}

void DrawAnimation::Kachujin3()
{
	kachujin3 = new ModelAnimator(shader);

	kachujin3->ReadMaterial(L"Kachujin/Mesh");
	kachujin3->ReadMesh(L"Kachujin/Mesh");
	kachujin3->ReadClip(L"Kachujin/Hip_Hop_Dancing");
	kachujin3->ReadClip(L"Kachujin/Idle");
	kachujin3->ReadClip(L"Kachujin/Running");


	temp = { Math::Random(-20.0f, 20.0f),0,Math::Random(-20.0f,20.0f) };
	kachujin3->GetTransform()->Position(temp.x, 0, temp.z);
	kachujin3->GetTransform()->Scale(0.025f, 0.025f, 0.025f);

	kachujin3->GetTransform()->Position(&Mpos[1]);

	collTransform3->Position(Mpos[1].x, Mpos[1].y + 2.5f, Mpos[1].z);
	collTransform3->Scale(2, 5, 2.3f);

	coll3 = new Collider(collTransform3);

	kachujin3->Pass(2);
}
