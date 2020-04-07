#pragma once

class Converter
{
public:
	Converter();
	~Converter();

	void ReadFile(wstring file);
	
	void ExportMatarial(wstring savePath, bool bOverwrite = true); //File 추출
	//bOverwirte 덮어쓰기 옵션 on off

	void ExportMesh(wstring savePath, bool bOverWirte = true);

	
private:
	//file 경로저장
	wstring file;

	//File 이전 scene부터 불러와야함
	Assimp::Importer* importer;
	const aiScene* scene;
	//Assimp에서 제공되는 변수 ai

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

