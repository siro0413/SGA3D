#pragma once
#include "Systems/IExecute.h"

class DrawAnimation : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Ready() {};
	virtual void Destroy();
	virtual void Update() override;
	virtual void PreRender() {};
	virtual void Render() override;
	virtual void PostRender() {};
	virtual void ResizeScreen() {};

private:
	void Kachujin();


	void Kachujin2();
	void Kachujin3();



	//Convert와 다르게 실제로 모델링 하는 함수
	Shader* shader;

	//실제 render하는곳
	Model* weapon;
	ModelAnimator* kachujin = NULL;
	ModelAnimator* kachujin2 = NULL;
	ModelAnimator* kachujin3 = NULL;
	float temp;
	Vector3 pos;
	Vector3 Mpos[2];

	Collider* coll = NULL;
	Collider* coll2 = NULL;
	Collider* coll3 = NULL;
	Transform* collTransform = new Transform;
	Transform* collTransform2 = new Transform;
	Transform* collTransform3 = new Transform;


	bool bCheck;
	bool bCheck2;
	bool test = false;
};
