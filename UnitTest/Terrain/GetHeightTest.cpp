#include "stdafx.h"
#include "GetHeightTest.h"
#include "Viewer/Freedom.h"
#include "Environment/Terrain.h"

void GetHeightTest::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(11, 0, 0);
	Context::Get()->GetCamera()->Position(132, 42, -17);
	((Freedom*)Context::Get()->GetCamera())->Speed(100, 2);

	shader = new Shader(L"11_Terrain.fx");

	terrain = new Terrain(shader, L"Terrain/Gray256.png");
	terrain->BaseMap(L"Terrain/Dirt3.png");

	meshShader = new Shader(L"06_Mesh.fx");
	sphere = new MeshSphere(meshShader, 0.5f);
	

}

void GetHeightTest::Destroy()
{
	SafeDelete(shader);
	SafeDelete(terrain);
}

void GetHeightTest::Update()
{
	Vector3 position;
	sphere->GetTranform()->Position(&position);

	if (Keyboard::Get()->Press(VK_UP))
		position.z += 20.0f * Time::Delta();
	else if (Keyboard::Get()->Press(VK_DOWN))
		position.z -= 20.0f * Time::Delta();

	if (Keyboard::Get()->Press(VK_LEFT))
		position.x -= 20.0f * Time::Delta();
	else if (Keyboard::Get()->Press(VK_RIGHT))
		position.x += 20.0f * Time::Delta();

	//Todo 절반 파묻히는거 방지

	position.y = terrain->GetHeightPick(position);
	sphere->GetTranform()->Position(position);

	static Vector3 light = Vector3(-1, -1, 1);
	ImGui::SliderFloat3("Light", light, -1, 1);
	shader->AsVector("LightDirection")->SetFloatVector(light);
	meshShader->AsVector("LightDirection")->SetFloatVector(light);

	terrain->Update();
}


void GetHeightTest::Render()
{	
	//terrain->Pass(1);
	terrain->Render();	
	sphere->Render();
}




