#include "stdafx.h"
#include "DrawModel.h"
#include "Viewer/Freedom.h"
#include "Utilities/Xml.h"
#include "MatrixSaveLoad.h"

void DrawModel::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(0, 0, 0);
	Context::Get()->GetCamera()->Position(0, 0, -50);
	((Freedom*)Context::Get()->GetCamera())->Speed(20, 2);

	for (size_t i = 0; i < 4; i++)
		bButton[i] = false;

	shader = new Shader(L"24_Model.fx");
	Airplane();
	Tower();
	Tank();
	Kachujin();
}

void DrawModel::Update()
{
	if (airPlane != NULL) airPlane->Update();
	if (tower != NULL) tower->Update();
	if (kachujin != NULL) kachujin->Update();
	if (tank != NULL) tank->Update();
	CheckBox();

}

void DrawModel::Render()
{
	if (airPlane != NULL) airPlane->Render();
	if (tower != NULL) tower->Render();
	if (tank != NULL) tank->Render();
	if (kachujin != NULL) kachujin->Render();

}

void DrawModel::Imgui(ModelRender* model)
{
	Vector3 rotation, position, scale;

	model->GetTransform()->RotationDegree(&rotation);
	model->GetTransform()->Scale(&scale);
	model->GetTransform()->Position(&position);


	ImGui::InputFloat3("Position", position);
	ImGui::InputFloat3("Rotation", rotation);
	ImGui::InputFloat3("Scale", scale);
	ImGui::Separator();




	model->GetTransform()->RotationDegree(rotation);
	model->GetTransform()->Scale(scale);
	model->GetTransform()->Position(position);
	model->Update();
}

void DrawModel::Reset(UINT index)
{
	Vector3 rotation, position, scale;
	switch (index)
	{
	case 0: //Airplane
	{
		airPlane->GetTransform()->Scale(0.005f, 0.005f, 0.005f);
		airPlane->GetTransform()->Position(0, 0, 0);
		airPlane->GetTransform()->RotationDegree(0, 0, 0);
		break;
	}
	case 1://Tower
	{
		tower->GetTransform()->Scale(0.01f, 0.01f, 0.01f);
		tower->GetTransform()->Position(-20, 0, 0);
		tower->GetTransform()->RotationDegree(0, 0, 0);
		break;
	}
	case 2://Kachujin
	{

		kachujin->GetTransform()->Scale(0.025f, 0.025f, 0.025f);
		kachujin->GetTransform()->Position(30, 0, 0);
		kachujin->GetTransform()->RotationDegree(0, 0, 0);
		break;
	}
	case 3://Tank
	{
		tank->GetTransform()->Scale(1, 1, 1);
		tank->GetTransform()->Position(20, 0, 0);
		tank->GetTransform()->RotationDegree(0, 0, 0);
		break;
	}
	default: temp = NULL;
		break;
	}
}

void DrawModel::Airplane()
{
	airPlane = new ModelRender(shader);
	airPlane->ReadMaterial(L"B787/Airplane");
	//Material을 불러온다

	airPlane->ReadMesh(L"B787/Airplane");
	airPlane->GetTransform()->Scale(0.005f, 0.005f, 0.005f);
}

void DrawModel::Tower()
{
	tower = new ModelRender(shader);
	tower->ReadMaterial(L"Tower/Tower");

	tower->ReadMesh(L"Tower/Tower");
	tower->GetTransform()->Position(-20, 0, 0);
	tower->GetTransform()->Scale(0.01f, 0.01f, 0.01f);
}

void DrawModel::Tank()
{
	tank = new ModelRender(shader);
	tank->ReadMaterial(L"Tank/Tank");

	tank->ReadMesh(L"Tank/Tank");
	tank->GetTransform()->Position(20, 0, 0);
	//bone의 index 갯수 확인하는법
	//vector<ModelBone*>& bones = tank->GetModel()->Bones();
}

void DrawModel::Kachujin()
{
	kachujin = new ModelRender(shader);
	kachujin->ReadMaterial(L"Kachujin/Mesh");

	kachujin->ReadMesh(L"Kachujin/Mesh");
	kachujin->GetTransform()->Position(30, 0, 0);
	kachujin->GetTransform()->Scale(0.025f, 0.025f, 0.025f);
}

void DrawModel::CheckBox()
{
	SaveLoad* sL = new SaveLoad();
	ImGui::Text("ModelName");
	for (size_t i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0: model = "Airplane"; temp = airPlane; break;
		case 1: model = "Tower"; temp = tower; break;
		case 2: model = "Kachujin";	temp = kachujin; break;
		case 3: model = "Tank";	temp = tank; break;
		default: temp = NULL;
			break;
		}

		if (!bButton[i])
		{
			ImGui::SameLine();
			if (ImGui::Button(model.c_str()))
				bButton[i] = true;
		}
		else if (bButton[i])
		{
			ImGui::SameLine();
			if (ImGui::Button(model.c_str()))
				bButton[i] = false;
		}

	}
	for (size_t i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0: model = "Airplane"; temp = airPlane; break;
		case 1: model = "Tower"; temp = tower; break;
		case 2: model = "Kachujin";	temp = kachujin; break;
		case 3: model = "Tank";	temp = tank; break;
		default: temp = NULL;
			break;
		}
		if (bButton[i])
		{
			ImGui::Separator();
			ImGui::Text(model.c_str());
			ImGui::SameLine();
			ImGui::Text("TransForm");

			Imgui(temp);
			if (ImGui::Button("Save"))
				sL->SaveMateraial(String::ToWString(this->model), temp->GetTransform());
			ImGui::SameLine();
			if (ImGui::Button("Load"))
				sL->LoadMateraial(String::ToWString(this->model), temp->GetTransform());
			ImGui::SameLine();
			if (ImGui::Button("Reset"))
				Reset(i);
		}
	}
	SafeDelete(sL);
}
