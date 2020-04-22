#pragma once
#include "Systems/IExecute.h"

class InstancingDemo : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Ready() override {};
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override {};
	virtual void Render() override;
	virtual void PostRender() override {};
	virtual void ResizeScreen() override {};

private:
	Shader* shader;
	Material* material;

	vector<Mesh::MeshVertex> vertices;
	vector<UINT> indices;

	VertexBuffer* vertexBuffer;
	VertexBuffer* instanceBuffer;
	IndexBuffer* indexBuffer;

	PerFrame* perFrame;

	Transform* transforms[500];
	Matrix worlds[500];
	//instances buffer타고 넘어갈애


	void CreateMesh();
	void AddTexture(wstring file);

	vector<wstring> textureFiles;
	TextureArray* textures = NULL;
	ID3DX11EffectShaderResourceVariable* sMaps;

};