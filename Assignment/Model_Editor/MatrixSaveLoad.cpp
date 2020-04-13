#include "stdafx.h"
#include "MatrixSaveLoad.h"
#include "Utilities/Xml.h"

void SaveLoad::SaveMateraial(wstring savePath, Transform * transform)
{
	file = savePath;
	savePath = L"../../_Save/" + savePath + L".xml";

	if (Path::ExistFile(savePath))
	{
		int messag = MessageBoxW(D3D::GetDesc().Handle, L"OverWrite?", file.c_str(), MB_YESNO);

		if (messag == IDYES)
			SaveFile(savePath, transform);

		else
			return;
	}
	else
		SaveFile(savePath, transform);

}

void SaveLoad::LoadMateraial(wstring loadPath, Transform * transform)
{
	file = loadPath;

	loadPath = L"../../_Save/" + loadPath + L".xml";

	if (Path::ExistFile(loadPath))
	{
		int messag = MessageBoxW(D3D::GetDesc().Handle, L"Load File", file.c_str(), MB_YESNO);

		if (messag == IDYES)
			LoadFile(loadPath, transform);

		else
			return;
	}
	else
		return;

}

void SaveLoad::SaveFile(wstring savePath, Transform * transform)
{
	string folder = String::ToString(Path::GetDirectoryName(savePath));
	string file = String::ToString(Path::GetFileName(savePath));

	//Transform불러오기..
	transform->Position(&pos);
	transform->Scale(&scale);
	transform->RotationDegree(&rotation);


	Path::CreateFolders(folder);
	Xml::XMLDocument* document = new Xml::XMLDocument();

	Xml::XMLDeclaration* decl = document->NewDeclaration();
	document->LinkEndChild(decl);

	Xml::XMLElement* root = document->NewElement(file.c_str());
	root->SetAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
	root->SetAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");
	document->LinkEndChild(root);


	Xml::XMLElement* node = document->NewElement("World");
	root->LinkEndChild(node);

	Xml::XMLElement* element = NULL;
	element = document->NewElement("Position");
	element->SetAttribute("X", pos.x);
	element->SetAttribute("Y", pos.y);
	element->SetAttribute("Z", pos.z);
	node->LinkEndChild(element);

	element = document->NewElement("Scale");
	element->SetAttribute("X", scale.x);
	element->SetAttribute("Y", scale.y);
	element->SetAttribute("Z", scale.z); 
	node->LinkEndChild(element);

	element = document->NewElement("Transform");
	element->SetAttribute("X", rotation.x);
	element->SetAttribute("Y", rotation.y);
	element->SetAttribute("Z", rotation.z);
	node->LinkEndChild(element);

	document->SaveFile((folder + file).c_str());
	SafeDelete(document);
}

void SaveLoad::LoadFile(wstring loadPath, Transform * transform)
{
	auto* document = new Xml::XMLDocument();
	Xml::XMLError error = document->LoadFile(String::ToString(loadPath).c_str());
	assert(error == Xml::XML_SUCCESS);

	auto* root = document->FirstChildElement(); 
	auto* world = root->FirstChildElement(); 

	Xml::XMLElement* node = NULL;

	node = world->FirstChildElement();

	wstring directory = Path::GetDirectoryName(loadPath);
	String::Replace(&directory, L"../Save", L"");
	Vector3 pos, scale, rotation;

	pos.x = node->FloatAttribute("X");
	pos.y = node->FloatAttribute("Y");
	pos.z = node->FloatAttribute("Z");
	transform->Position(pos);

	node = node->NextSiblingElement();
	scale.x = node->FloatAttribute("X");
	scale.y = node->FloatAttribute("Y");
	scale.z = node->FloatAttribute("Z");
	transform->Scale(scale);

	node = node->NextSiblingElement();
	rotation.x = node->FloatAttribute("X");
	rotation.y = node->FloatAttribute("Y");
	rotation.z = node->FloatAttribute("Z");
	transform->RotationDegree(rotation);

	SafeDelete(document);
}
