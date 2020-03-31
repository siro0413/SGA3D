#pragma once

class SkyCube
{
public:

public:
	SkyCube(wstring file);
	~SkyCube();

	void Update();
	void Render();

private:	
	Shader* shader;
	MeshSphere* sphere;

	ID3D11ShaderResourceView* srv;
	ID3DX11EffectShaderResourceVariable* sSrv;

};