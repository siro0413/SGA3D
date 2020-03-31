#include "Framework.h"
#include "Freedom.h"

Freedom::Freedom()
	: Camera()
{
}

Freedom::~Freedom()
{
}

void Freedom::Update()
{
	if (Mouse::Get()->Press(1) == false) return;

	Vector3 f = Forward();
	Vector3 u = Up();
	Vector3 r = Right();

	//�̵�
	{
		Vector3 P;
		Position(&P);

		if (Keyboard::Get()->Press('W'))
			P = P + f * move * Time::Delta();
		else if (Keyboard::Get()->Press('S'))
			P = P - f * move * Time::Delta();

		if (Keyboard::Get()->Press('D'))
			P = P + r * move * Time::Delta();
		else if (Keyboard::Get()->Press('A'))
			P = P - r * move * Time::Delta();

		if (Keyboard::Get()->Press('E'))
			P = P + u * move * Time::Delta();
		else if (Keyboard::Get()->Press('Q'))
			P = P - u * move * Time::Delta();

		Position(P);
	}

	//ȸ��
	{
		Vector3 R;
		Rotation(&R);

		Vector3 val = Mouse::Get()->GetMoveValue();

		R.x += val.y * rotation * Time::Delta();
		R.y += val.x * rotation * Time::Delta();
		R.z = 0.0f;

		Rotation(R);
	}
	
}

void Freedom::Speed(float move, float rotation)
{
	this->move = move;
	this->rotation = rotation;
}
