#pragma once
#include "Systems/IExecute.h"

class MatrixSaveLoad;

class DrawModel : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Ready() {};
	virtual void Destroy() {};
	virtual void Update() override;
	virtual void PreRender() {};
	virtual void Render() override;
	virtual void PostRender() {};
	virtual void ResizeScreen() {};


	void Imgui(ModelRender* model);
	void Reset(UINT index);
private:
	void Airplane();
	void Tower();
	void Tank();
	void Kachujin();
	//Convert와 다르게 실제로 모델링 하는 함수
	
	void CheckBox();
	Shader* shader;
	bool bButton[4];

	ModelRender* airPlane = NULL;
	//실제 render하는곳
	ModelRender* tower = NULL;
	ModelRender* tank = NULL;
	ModelRender* kachujin = NULL;
	ModelRender* temp = NULL;
	string model;

};
