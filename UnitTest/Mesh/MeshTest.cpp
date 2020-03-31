#include "stdafx.h"
#include "MeshTest.h"
#include "Viewer/Freedom.h"


void MeshTest::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(23, 0, 0);
	Context::Get()->GetCamera()->Position(0, 32, -67);
	((Freedom*)Context::Get()->GetCamera())->Speed(20, 2);

	shader = new Shader(L"06_Mesh.fx");

	cube = new MeshCube(shader);
	cube->GetTranform()->Position(0, 5, 0);
	cube->GetTranform()->Scale(20, 10, 20);
	

	grid = new MeshGrid(shader);
	grid->GetTranform()->Scale(20, 1, 20);
	

	for (UINT i = 0; i < 5; i++)
	{
		cylinder[i * 2] = new MeshCylinder(shader, 0.5f, 3.0f, 20, 20);
		cylinder[i * 2]->GetTranform()->Position(-30, 6, -15.0f + (float)i * 15.0f);
		cylinder[i * 2]->GetTranform()->Scale(5, 5, 5);
		

		cylinder[i * 2 + 1] = new MeshCylinder(shader, 0.5f, 3.0f, 20, 20);
		cylinder[i * 2 + 1]->GetTranform()->Position(30, 6, -15.0f + (float)i * 15.0f);
		cylinder[i * 2 + 1]->GetTranform()->Scale(5, 5, 5);
		

		sphere[i * 2] = new MeshSphere(shader, 0.5f, 20, 20);
		sphere[i * 2]->GetTranform()->Position(-30, 15.5f, -15.0f + (float)i * 15.0f);
		sphere[i * 2]->GetTranform()->Scale(5, 5, 5);
		

		sphere[i * 2 + 1] = new MeshSphere(shader, 0.5f, 20, 20);
		sphere[i * 2 + 1]->GetTranform()->Position(30, 15.5f, -15.0f + (float)i * 15.0f);
		sphere[i * 2 + 1]->GetTranform()->Scale(5, 5, 5);
		
	}
}

void MeshTest::Destroy()
{
	SafeDelete(cube);
	SafeDelete(shader);

	for (MeshCylinder* c : cylinder)
		SafeDelete(c);
	for (MeshSphere* s : sphere)
		SafeDelete(s);
	
}

void MeshTest::Update()
{
	
}


void MeshTest::Render()
{	
	cube->Render();
	grid->Render();

	for (MeshCylinder* c : cylinder)
		c->Render();
	for (MeshSphere* s : sphere)
		s->Render();
	
}




