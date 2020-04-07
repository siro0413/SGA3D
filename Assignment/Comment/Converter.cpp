#include "stdafx.h"
#include "Converter.h"
#include "Types.h"
#include "Utilities/Xml.h"
#include "Utilities/BinaryFile.h"

Converter::Converter()
{
	importer = new Assimp::Importer();

}

Converter::~Converter()
{
	SafeDelete(importer);
}

void Converter::ReadFile(wstring file)
{
	this->file = L"../../_Assets/" + file;

	scene = importer->ReadFile
	(
		String::ToString(this->file),
		aiProcess_ConvertToLeftHanded // <- 오른손을 왼손 자표계로 변환해주는 매크로
		| aiProcess_Triangulate //폴리곤단위로 불러오겠다
		| aiProcess_GenUVCoords //모델형식에 맞게 uv좌표변환
		| aiProcess_GenNormals //normal vector 다시변환
		| aiProcess_CalcTangentSpace //전반사광
	);
	assert(scene != NULL);
}

void Converter::ExportMatarial(wstring savePath, bool bOverwrite)
{
	savePath = L"../../_Textures/" + savePath + L".material";

	ReadMaterial();
	WriteMaterial(savePath, bOverwrite);
}

void Converter::ExportMesh(wstring savePath, bool bOverWirte)
{
	savePath = L"../../_Models/" + savePath + L".mesh";

	ReadBoneData(scene->mRootNode, -1, -1);//-1은 처음으로 넘어왔다 (최초호출)
	//부모가 -1이면 부모가 없다(최상위 루트다)

	WriteMeshData(savePath, bOverWirte);
}

void Converter::ReadMaterial()
{
	//Scene에 있는 Materail을 전부 불러오기
	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* srcMaterial = scene->mMaterials[i];
		asMaterial* matarial = new asMaterial();

		matarial->Name = srcMaterial->GetName().C_Str(); //ai Data copy for as data

		aiColor3D color;

		srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color); //
		//AI_MATKEY_COLOR_AMBIENT "$clr.ambient", 0, 0 인자 3개역활
		matarial->Ambient = Color(color.r, color.g, color.b, 1);

		srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		matarial->Diffuse = Color(color.r, color.g, color.b, 1);

		srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
		matarial->Specular = Color(color.r, color.g, color.b, 1);

		srcMaterial->Get(AI_MATKEY_SHININESS, matarial->Specular.a);

		aiString file;

		srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
		matarial->DiffuseFile = file.C_Str();

		srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
		matarial->SpecularFile = file.C_Str();

		srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
		matarial->NormalFile = file.C_Str();


		materials.push_back(matarial);
	}
}

void Converter::WriteMaterial(wstring savePath, bool bOverWrite)
{
	if (!bOverWrite)
	{
		if (Path::ExistFile(savePath))
			return;
	}

	string folder = String::ToString(Path::GetDirectoryName(savePath));
	string file = String::ToString(Path::GetFileName(savePath));

	Path::CreateFolders(folder);

	Xml::XMLDocument* document = new Xml::XMLDocument();

	Xml::XMLDeclaration* decl = document->NewDeclaration();
	document->LinkEndChild(decl);

	Xml::XMLElement* root = document->NewElement("Materials");
	root->SetAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
	root->SetAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");
	document->LinkEndChild(root);

	for (asMaterial* material : materials)
	{//Tree 생성
		Xml::XMLElement* node = document->NewElement("Material");
		root->LinkEndChild(node);

		Xml::XMLElement* element = NULL;

		element = document->NewElement("Name");
		element->SetText(material->Name.c_str());
		node->LinkEndChild(element);

		element = document->NewElement("DiffuseFile");
		element->SetText(WriteTexture(folder, material->DiffuseFile).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("SpecularFile");
		element->SetText(WriteTexture(folder, material->SpecularFile).c_str());
		node->LinkEndChild(element);

		element = document->NewElement("NormalFile");
		element->SetText(WriteTexture(folder, material->NormalFile).c_str());
		node->LinkEndChild(element);



		element = document->NewElement("Ambient");
		element->SetAttribute("R", material->Ambient.r);
		element->SetAttribute("G", material->Ambient.g);
		element->SetAttribute("B", material->Ambient.b);
		element->SetAttribute("A", material->Ambient.a);
		node->LinkEndChild(element);

		element = document->NewElement("Diffuse");
		element->SetAttribute("R", material->Diffuse.r);
		element->SetAttribute("G", material->Diffuse.g);
		element->SetAttribute("B", material->Diffuse.b);
		element->SetAttribute("A", material->Diffuse.a);
		node->LinkEndChild(element);

		element = document->NewElement("Specular");
		element->SetAttribute("R", material->Specular.r);
		element->SetAttribute("G", material->Specular.g);
		element->SetAttribute("B", material->Specular.b);
		element->SetAttribute("A", material->Specular.a);
		node->LinkEndChild(element);

		//Texture 저장
		SafeDelete(material);
	}

	document->SaveFile((folder + file).c_str());
	SafeDelete(document);
}

string Converter::WriteTexture(string savePath, string file)
{
	if (file.length() < 1) return "";

	string fileName = Path::GetFileName(file);
	//../../다날리고 파일명만 가져옴

	const aiTexture* texture = scene->GetEmbeddedTexture(file.c_str());
	//내장 외장 텍스쳐 확인 코드

	string path = "";
	if (texture != NULL) //Texture File != NULL
	{
		//model 안에 텍스쳐 저장되어있는경우
		path = savePath + Path::GetFileNameWithoutExtension(file) + ".png";

		if (texture->mHeight < 1) //1차원 데이터
		{
			BinaryWriter w;
			w.Open(String::ToWString(path));
			w.BYTE(texture->pcData, texture->mWidth);//시작주소, 끝;
			w.Close();
		}
		else
		{
			ID3D11Texture2D* dest;
			D3D11_TEXTURE2D_DESC desc;
			ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
			desc.Width = texture->mWidth;
			desc.Height = texture->mHeight;
			desc.MipLevels = 1; //확대 축소
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //unsigned Normal = 0~1의 값만 가지며 R~A의 각각 8비트 총 32비트
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_IMMUTABLE;//CPU GPU Read Write false

			D3D11_SUBRESOURCE_DATA subResource = { 0 };
			subResource.pSysMem = texture->pcData;

			HRESULT hr;
			hr = D3D::GetDevice()->CreateTexture2D(&desc, &subResource, &dest);

			Check(hr);

			D3DX11SaveTextureToFile(D3D::GetDC(), dest, D3DX11_IFF_PNG, savePath.c_str());
			//Png File Save
		}
	}
	else //(texture != NULL)
	{
		string directory = Path::GetDirectoryName(String::ToString(this->file));
		string origin = directory + file;
		String::Replace(&origin, "\\", "/");
		// \\을 /로 변경
		if (!Path::ExistDirectory(origin))
			return "";

		path = savePath + fileName;
		CopyFileA(origin.c_str(), path.c_str(), FALSE);

		String::Replace(&path, "../../_Textures/", "");
		//../../_Textures/ 경로 delete
	}

	return Path::GetFileName(path);
}

void Converter::ReadBoneData(aiNode * node, int index, int parent)
{
	//모든 트리노드를 찾아야되니 재귀호출
	asBone* bone = new asBone();
	bone->Index = index;
	bone->Parent = parent;
	bone->Name = node->mName.C_Str();

	Matrix transform(node->mTransformation[0]);//배열의 시작주소를 넣으면 생성자로 자동 초기화
	D3DXMatrixTranspose(&bone->Transform, &transform);

	Matrix matParent;
	if (parent < 0) //if 자기자신이 최상위 부모라면
		D3DXMatrixIdentity(&matParent);
	//부모를 기본 월드로 만든다.
	else
		matParent = bones[parent]->Transform;
	//부모가 아닐시 matparent에 저장해준뒤
	bone->Transform = bone->Transform * matParent;
	//부모의 공간에 붙여준다.

	bones.push_back(bone);

	ReadMeshData(node, index);
	//mesh에대한 material정보들이 필요

	for (size_t i = 0; i < node->mNumChildren; i++) //bone에해당하는 자식수만큼 재귀 호출
		ReadBoneData(node->mChildren[i], bones.size(), index);
	//재귀호출될때마다 index가 1++되고 다음자식이 붙는다
}

void Converter::ReadMeshData(aiNode * node, int bone)
{
	if (node->mNumMeshes < 1) return;

	asMesh* mesh = new asMesh();
	mesh->Name = node->mName.C_Str();
	mesh->Boneindex = bone;

	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		UINT index = node->mMeshes[i];
		aiMesh* srcMesh = scene->mMeshes[index];
		//실제 데이터는 scene에 존재

		aiMaterial* material = scene->mMaterials[srcMesh->mMaterialIndex];
		mesh->MaterialName = material->GetName().C_Str();

		UINT startVertex = mesh->Vertices.size();
		for (size_t v = 0; v < srcMesh->mNumVertices; v++)
		{
			Model::ModelVertex vertex;
			//변수 6개를 다시저장 갯수를 세기 어려우니 vector로 저장
			memcpy(&vertex.Position, &srcMesh->mVertices[v], sizeof(Vector3));
			//vertex 위치정보 uv

			if (srcMesh->HasTextureCoords(0))
				memcpy(&vertex.Uv, &srcMesh->mTextureCoords[0][v], sizeof(Vector2));
			//mTextureCoords[0] = layer 갯수

			if (srcMesh->HasNormals())
				memcpy(&vertex.Normal, &srcMesh->mNormals[v], sizeof(Vector3));

			if (srcMesh->HasTangentsAndBitangents())
				memcpy(&vertex.Tangent, &srcMesh->mTangents[v], sizeof(Vector3));

			mesh->Vertices.push_back(vertex);
		}//(size_t v = 0; v < srcMesh->mNumAnimMeshes; v++)

		//model에서 index = 면단위 (사각형 or 삼각형) face단위
		//face단위를 인덱스로 변환

		for (size_t f = 0; f < srcMesh->mNumFaces; f++)//mNumface = 면의 갯수
		{
			aiFace& face = srcMesh->mFaces[f];

			for (size_t k = 0; k < face.mNumIndices; k++)
			{
				mesh->Indices.push_back(face.mIndices[k]);
				mesh->Indices.back() += startVertex;
				//startVertex의 size; vertices의 사이즈만큼 늘어난다.
			}
		}//(size_t f = 0; f < srcMesh->mNumFaces; f++)

		meshs.push_back(mesh);
	}//(size_t i = 0; i < node->mNumMeshes; i++)

}

void Converter::WriteMeshData(wstring savePath, bool bOverWrite)
{
	if (!bOverWrite)
		if (Path::ExistFile(savePath))
			return;

	Path::CreateFolders(Path::GetDirectoryName(savePath));

	BinaryWriter* w = new BinaryWriter();

	w->Open(savePath);
	//없으면 만들어줌

	w->UInt(bones.size());

	for (asBone* bone : bones)
	{
		w->Int(bone->Index);
		w->String(bone->Name);
		w->Int(bone->Parent);
		w->Matrix(bone->Transform);

		SafeDelete(bone);
	}

	w->UInt(meshs.size());
	for (asMesh* mesh : meshs)
	{
		w->String(mesh->Name);
		w->Int(mesh->Boneindex);

		w->String(mesh->MaterialName);

		w->UInt(mesh->Vertices.size());
		//정정갯수
		w->BYTE(&mesh->Vertices[0], sizeof(Model::ModelVertex) * mesh->Vertices.size());
		//정점의 정보
		w->BYTE(&mesh->Indices[0], sizeof(UINT) * mesh->Indices.size());

		SafeDelete(mesh);
	}
	w->Close();
	SafeDelete(w);
}
