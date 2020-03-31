#pragma once
#include "Camera.h"

class Freedom : public Camera
{
public:
	Freedom();
	~Freedom();

	void Update() override;
	void Speed(float move, float rotation);

private:
	float move = 20.0f; //이동 속도
	float rotation = 2.0f; //회전 속도
};