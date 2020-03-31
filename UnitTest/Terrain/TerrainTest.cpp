#include "stdafx.h"
#include "TerrainTest.h"
#include "Environment/Terrain.h"

void TerrainTest::Initialize()
{
	shader = new Shader(L"05_Light.fx");

	terrain = new Terrain(shader, L"Terrain/Gray256.png");

}

void TerrainTest::Destroy()
{
	SafeDelete(shader);
	SafeDelete(terrain);
}

void TerrainTest::Update()
{
	static Vector3 light = Vector3(-1, -1, 1);
	ImGui::SliderFloat3("Light", light, -1, 1);
	shader->AsVector("LightDirection")->SetFloatVector(light);

	terrain->Update();
}


void TerrainTest::Render()
{	
	//terrain->Pass(1);
	terrain->Render();	
}




