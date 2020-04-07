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
		aiProcess_ConvertToLeftHanded // <- �������� �޼� ��ǥ��� ��ȯ���ִ� ��ũ��
		| aiProcess_Triangulate //����������� �ҷ����ڴ�
		| aiProcess_GenUVCoords //�����Ŀ� �°� uv��ǥ��ȯ
		| aiProcess_GenNormals //normal vector �ٽú�ȯ
		| aiProcess_CalcTangentSpace //���ݻ籤
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

	ReadBoneData(scene->mRootNode, -1, -1);//-1�� ó������ �Ѿ�Դ� (����ȣ��)
	//�θ� -1�̸� �θ� ����(�ֻ��� ��Ʈ��)

	WriteMeshData(savePath, bOverWirte);
}

void Converter::ReadMaterial()
{
	//Scene�� �ִ� Materail�� ���� �ҷ�����
	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* srcMaterial = scene->mMaterials[i];
		asMaterial* matarial = new asMaterial();

		matarial->Name = srcMaterial->GetName().C_Str(); //ai Data copy for as data

		aiColor3D color;

		srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color); //
		//AI_MATKEY_COLOR_AMBIENT "$clr.ambient", 0, 0 ���� 3����Ȱ
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
	{//Tree ����
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

		//Texture ����
		SafeDelete(material);
	}

	document->SaveFile((folder + file).c_str());
	SafeDelete(document);
}

string Converter::WriteTexture(string savePath, string file)
{
	if (file.length() < 1) return "";

	string fileName = Path::GetFileName(file);
	//../../�ٳ����� ���ϸ� ������

	const aiTexture* texture = scene->GetEmbeddedTexture(file.c_str());
	//���� ���� �ؽ��� Ȯ�� �ڵ�

	string path = "";
	if (texture != NULL) //Texture File != NULL
	{
		//model �ȿ� �ؽ��� ����Ǿ��ִ°��
		path = savePath + Path::GetFileNameWithoutExtension(file) + ".png";

		if (texture->mHeight < 1) //1���� ������
		{
			BinaryWriter w;
			w.Open(String::ToWString(path));
			w.BYTE(texture->pcData, texture->mWidth);//�����ּ�, ��;
			w.Close();
		}
		else
		{
			ID3D11Texture2D* dest;
			D3D11_TEXTURE2D_DESC desc;
			ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
			desc.Width = texture->mWidth;
			desc.Height = texture->mHeight;
			desc.MipLevels = 1; //Ȯ�� ���
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //unsigned Normal = 0~1�� ���� ������ R~A�� ���� 8��Ʈ �� 32��Ʈ
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
		// \\�� /�� ����
		if (!Path::ExistDirectory(origin))
			return "";

		path = savePath + fileName;
		CopyFileA(origin.c_str(), path.c_str(), FALSE);

		String::Replace(&path, "../../_Textures/", "");
		//../../_Textures/ ��� delete
	}

	return Path::GetFileName(path);
}

void Converter::ReadBoneData(aiNode * node, int index, int parent)
{
	//��� Ʈ����带 ã�ƾߵǴ� ���ȣ��
	asBone* bone = new asBone();
	bone->Index = index;
	bone->Parent = parent;
	bone->Name = node->mName.C_Str();

	Matrix transform(node->mTransformation[0]);//�迭�� �����ּҸ� ������ �����ڷ� �ڵ� �ʱ�ȭ
	D3DXMatrixTranspose(&bone->Transform, &transform);

	Matrix matParent;
	if (parent < 0) //if �ڱ��ڽ��� �ֻ��� �θ���
		D3DXMatrixIdentity(&matParent);
	//�θ� �⺻ ����� �����.
	else
		matParent = bones[parent]->Transform;
	//�θ� �ƴҽ� matparent�� �������ص�
	bone->Transform = bone->Transform * matParent;
	//�θ��� ������ �ٿ��ش�.

	bones.push_back(bone);

	ReadMeshData(node, index);
	//mesh������ material�������� �ʿ�

	for (size_t i = 0; i < node->mNumChildren; i++) //bone���ش��ϴ� �ڽļ���ŭ ��� ȣ��
		ReadBoneData(node->mChildren[i], bones.size(), index);
	//���ȣ��ɶ����� index�� 1++�ǰ� �����ڽ��� �ٴ´�
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
		//���� �����ʹ� scene�� ����

		aiMaterial* material = scene->mMaterials[srcMesh->mMaterialIndex];
		mesh->MaterialName = material->GetName().C_Str();

		UINT startVertex = mesh->Vertices.size();
		for (size_t v = 0; v < srcMesh->mNumVertices; v++)
		{
			Model::ModelVertex vertex;
			//���� 6���� �ٽ����� ������ ���� ������ vector�� ����
			memcpy(&vertex.Position, &srcMesh->mVertices[v], sizeof(Vector3));
			//vertex ��ġ���� uv

			if (srcMesh->HasTextureCoords(0))
				memcpy(&vertex.Uv, &srcMesh->mTextureCoords[0][v], sizeof(Vector2));
			//mTextureCoords[0] = layer ����

			if (srcMesh->HasNormals())
				memcpy(&vertex.Normal, &srcMesh->mNormals[v], sizeof(Vector3));

			if (srcMesh->HasTangentsAndBitangents())
				memcpy(&vertex.Tangent, &srcMesh->mTangents[v], sizeof(Vector3));

			mesh->Vertices.push_back(vertex);
		}//(size_t v = 0; v < srcMesh->mNumAnimMeshes; v++)

		//model���� index = ����� (�簢�� or �ﰢ��) face����
		//face������ �ε����� ��ȯ

		for (size_t f = 0; f < srcMesh->mNumFaces; f++)//mNumface = ���� ����
		{
			aiFace& face = srcMesh->mFaces[f];

			for (size_t k = 0; k < face.mNumIndices; k++)
			{
				mesh->Indices.push_back(face.mIndices[k]);
				mesh->Indices.back() += startVertex;
				//startVertex�� size; vertices�� �����ŭ �þ��.
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
	//������ �������

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
		//��������
		w->BYTE(&mesh->Vertices[0], sizeof(Model::ModelVertex) * mesh->Vertices.size());
		//������ ����
		w->BYTE(&mesh->Indices[0], sizeof(UINT) * mesh->Indices.size());

		SafeDelete(mesh);
	}
	w->Close();
	SafeDelete(w);
}
