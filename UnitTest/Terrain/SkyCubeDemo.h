#pragma once
#include "Systems/IExecute.h"

class SkyCubeDemo : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Ready() override {}
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override {}
	virtual void Render() override;
	virtual void PostRender() override {}
	virtual void ResizeScreen() override {}

private:
	Shader * shader;

	MeshCube* cube;
	MeshCylinder* cylinder[10];
	MeshSphere* sphere[10];

	MeshGrid* grid;

	Shader* terrainShader; //
	class Terrain* terrain; //

	class SkyCube* sky;
};