#pragma once

class PerFrame
{
public:
	PerFrame(Shader* shader);
	~PerFrame();

public:
	void Update();
	void Render();

private:
	struct BufferDesc
	{
		Matrix View;
		Matrix ViewInverse;
		Matrix Projection;
		Matrix VP;

		float Time;
		float Padding[3];
	} bufferDesc;

	struct LightDesc
	{
		Color Ambient;
		Color Specular;
		Vector3 Direction;
		float Padding;
		Vector3 Position;
		float Padding2;
	} lightDesc;

private:
	Shader * shader;

	ConstantBuffer* buffer;
	ID3DX11EffectConstantBuffer* sBuffer;
	
	ConstantBuffer* lightBuffer;
	ID3DX11EffectConstantBuffer* sLightBuffer;
};
