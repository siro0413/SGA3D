#pragma once

class PlayerModel
{
public:
	PlayerModel(Shader* shader);
	~PlayerModel();
	
	void Update();
	void Render();
	void Move();

	auto* PlayerCollider() { return coll; }
	bool& SetBool(bool val) {return bCheck = val; }

private:
	ModelAnimator* player;
	Transform* cTrans;
	Collider* coll;

	bool bCheck;

};
