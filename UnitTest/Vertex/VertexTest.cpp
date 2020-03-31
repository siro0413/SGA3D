#include "stdafx.h"
#include "VertexTest.h"

void VertexTest::Initialize()
{
	shader = new Shader(L"01_Vertex.fx");

	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f);
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f);
	vertices[2].Position = Vector3(+0.5f, -0.5f, 0.0f);
	vertices[3].Position = Vector3(+0.5f, -0.5f, 0.0f);
	vertices[4].Position = Vector3(-0.5f, +0.5f, 0.0f);
	vertices[5].Position = Vector3(+0.5f, +0.5f, 0.0f);

	vertices[0].Color = Color(1, 0, 0, 0);
	vertices[1].Color = Color(0, 1, 0, 0);
	vertices[2].Color = Color(0, 0, 1, 0);
	vertices[3].Color = Color(1, 0, 0, 0);
	vertices[4].Color = Color(0, 1, 0, 0);
	vertices[5].Color = Color(0, 0, 1, 0);

	//Create Buffer
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = sizeof(VertexColor) * 6;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = vertices;

	Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer));
}

void VertexTest::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertexBuffer);
}

void VertexTest::Update()
{
}


void VertexTest::Render()
{
	UINT stride = sizeof(VertexColor);
	UINT offset = 0;

	static bool bWireFrame = false;
	ImGui::Checkbox("WireFrame", &bWireFrame);

	static Color color = Color(1, 1, 1, 1);
	ImGui::ColorEdit3("Color", color);
	shader->AsVector("Color")->SetFloatVector(color);

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->Draw(0, bWireFrame == false ? 0 : 1, 6);
}



