#pragma once

class Converter
{
public:
	Converter();
	~Converter();

	void ReadFile(wstring file);
	
	void ExportMatarial(wstring savePath, bool bOverwrite = true); //File ����
	//bOverwirte ����� �ɼ� on off

	void ExportMesh(wstring savePath, bool bOverWirte = true);

	
private:
	//file �������
	wstring file;

	//File ���� scene���� �ҷ��;���
	Assimp::Importer* importer;
	const aiScene* scene;
	//Assimp���� �����Ǵ� ���� ai

	vector<struct asMaterial*> materials;
	vector<struct asBone*> bones;

	vector<struct asMesh*> meshs;


	void ReadMaterial();
	void WriteMaterial(wstring savePath, bool bOverWrite = true);
	string WriteTexture(string savePath, string file);

	void ReadBoneData(aiNode* node, int index, int parent);
	void ReadMeshData(aiNode* node, int bone);
	void WriteMeshData(wstring savePath, bool bOverWrite = true);
};

