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
	void Weapon_();
	class Weapon* we[9];


	//Convert�� �ٸ��� ������ �𵨸� �ϴ� �Լ�
	Shader* shader;
	vector<wstring> weapos;

	//���� render�ϴ°�
	ModelRender* weapons[9];
	Model* weapon;
	ModelAnimator* kachujin = NULL;
	Vector3 pos;
	Transform* collTransform = new Transform;
	Collider* coll = NULL;
	Vector3 pos2;

	bool bCheck[9];
	bool bWeapon = false;
};
