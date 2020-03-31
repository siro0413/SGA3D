#include "stdafx.h"
#include "TerrainBrushDemo.h"
#include "Viewer/Freedom.h"
#include "Environment/Terrain.h"

void TerrainBrushDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(11, 0, 0);
	Context::Get()->GetCamera()->Position(132, 42, -17);
	((Freedom*)Context::Get()->GetCamera())->Speed(100, 2);

	shader = new Shader(L"21_TerrainBrush.fx");

	terrain = new Terrain(shader, L"Terrain/Gray256.png");
	terrain->BaseMap(L"Terrain/Dirt3.png");
}

void TerrainBrushDemo::Destroy()
{
	SafeDelete(shader);
	SafeDelete(terrain);
}

void TerrainBrushDemo::Update()
{
	terrain->Update();
}


void TerrainBrushDemo::Render()
{	
	//terrain->Pass(1);
	terrain->Render();	
}




