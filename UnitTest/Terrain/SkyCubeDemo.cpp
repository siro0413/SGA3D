#include "stdafx.h"
#include "SkyCubeDemo.h"
#include "Viewer/Freedom.h"
#include "Environment/Terrain.h"
#include "Environment/SkyCube.h"
void SkyCubeDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(23, 0, 0);
	Context::Get()->GetCamera()->Position(0, 32, -67);
	((Freedom *)Context::Get()->GetCamera())->Speed(20, 2);


	shader = new Shader(L"14_Mesh.fx"); //<¸¸¾à ½¦ÀÌ´õ 09¸é... ½ÅÀüÀÌ »¡°²°Ô ³ª¿È

	cube = new MeshCube(shader);
	cube->GetTranform()->Position(0, 5, 0);
	cube->GetTranform()->Scale(20, 10, 20);
	cube->DiffuseMap(L"Stones.png"); //

	grid = new MeshGrid(shader);
	//grid = new MeshGrid(shader, 5, 5);
	grid->GetTranform()->Scale(20, 1, 20);
	grid->DiffuseMap(L"Floor.png"); //


	for (UINT i = 0; i < 5; i++)
	{
		cylinder[i * 2] = new MeshCylinder(shader, 0.5f, 3.0f, 20, 20);
		cylinder[i * 2]->GetTranform()->Position(-30, 6, -15.0f + (float)i * 15.0f);
		cylinder[i * 2]->GetTranform()->Scale(5, 5, 5);
		cylinder[i * 2]->DiffuseMap(L"Bricks.png"); //

		cylinder[i * 2 + 1] = new MeshCylinder(shader, 0.5f, 3.0f, 20, 20);
		cylinder[i * 2 + 1]->GetTranform()->Position(30, 6, -15.0f + (float)i * 15.0f);
		cylinder[i * 2 + 1]->GetTranform()->Scale(5, 5, 5);
		cylinder[i * 2 + 1]->DiffuseMap(L"Bricks.png"); //


		sphere[i * 2] = new MeshSphere(shader, 0.5f, 20, 20);
		sphere[i * 2]->GetTranform()->Position(-30, 15.5f, -15.0f + (float)i * 15.0f);
		sphere[i * 2]->GetTranform()->Scale(5, 5, 5);
		sphere[i * 2]->DiffuseMap(L"Wall.png"); //

		sphere[i * 2 + 1] = new MeshSphere(shader, 0.5f, 20, 20);
		sphere[i * 2 + 1]->GetTranform()->Position(30, 15.5f, -15.0f + (float)i * 15.0f);
		sphere[i * 2 + 1]->GetTranform()->Scale(5, 5, 5);
		sphere[i * 2 + 1]->DiffuseMap(L"Wall.png"); //
	}


	sky = new SkyCube(L"Environment/GrassCube1024.dds");

	terrainShader = new Shader(L"11_Terrain.fx");//
	terrain = new Terrain(terrainShader, L"Terrain/Gray256.png");//
	terrain->BaseMap(L"Terrain/Dirt3.png");//
}

void SkyCubeDemo::Destroy()
{

}

void SkyCubeDemo::Update()
{
	sky->Update();
	terrain->Update();
}

void SkyCubeDemo::Render()
{
	sky->Render();

	for (UINT i = 0; i < 10; i++)
		cylinder[i]->Render();

	for (UINT i = 0; i < 10; i++)
		sphere[i]->Render();

	cube->Render();
	grid->Render();

	terrain->Render(); //
}