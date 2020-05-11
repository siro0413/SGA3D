#pragma once
#include "Systems/IExecute.h"

class ShadowDemo : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Ready() override {}
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override {}
	virtual void ResizeScreen() override {}

private:
	Shader* shader;

	Render2D* shadow2D;
	Shadow* shadow;

	class SkyCube* sky;

	Material* floor;
	Material* stone;
	Material* brick;
	Material* wall;

	MeshRender* sphere;
	MeshRender* cylinder;
	MeshRender* cube;
	MeshRender* grid;

	Model* weapon;
	ModelAnimator* kachujin = NULL;
	ModelAnimator* ganful = NULL;

	struct ColliderDesc
	{
		Transform* Init; //collider의 크기라던지 위치 조정
		Transform* Transform;
		Collider* Collider;
	}collider[4];

	vector<MeshRender*> meshs;
	vector<ModelRender*> models;
	vector<ModelAnimator*> animators;
	Vector3 gPos, gPos2,gPos3;


	void Mesh();
	void Kachujin();
	void Ganfaul();

	void Move();
	void MonsterMove(ModelAnimator* model);


	void Pass(UINT mesh, UINT model, UINT animation);
};