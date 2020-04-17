#pragma once
#include "Systems/IExecute.h"

class DrawAni : public IExecute
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
	Shader* shader;
	class EnemyModel* enemy[3];
	class PlayerModel* player;

};
