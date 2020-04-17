#pragma once

class EnemyModel
{
public:
	EnemyModel(Shader* shader);
	~EnemyModel();

	void Update();
	void Render();

	void AutoMove();

	void SetIntersect(Collider* coll) { bCheck = this->coll->IsIntersect(coll); }


	auto* GetModel() { return enemy; }
	auto* GetCollider() { return coll; }
	auto GetBool() { return bCheck; }
private:

	ModelAnimator* enemy;
	Collider* coll;
	Transform* cTrans;
	Vector3 rPos2, rPos;
	int clip;
	bool bCheck;
};

