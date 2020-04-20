#pragma once

class Weapon
{
public:
	Weapon(Shader* shader, wstring file);
	~Weapon();

	void Update();
	void Render();
	
	auto* WeaponCollider() { return coll; }
	auto* GetTrransform() { return cTrans; }
	void SetIntersect(Collider* coll) { bCheck = this->coll->IsIntersect(coll); }
	bool GetBool() { return bCheck; }
private:
	ModelRender* weapon;
	Transform* cTrans;
	Collider* coll;

	bool bCheck = false;

};
