#include "Framework.h"
#include "Context.h"
#include "Viewer/Viewport.h"
#include "Viewer/Perspective.h"

#include "Viewer/Freedom.h"

Context* Context::instance = NULL;

Context * Context::Get()
{
	return instance;
}

void Context::Create()
{
	assert(instance == NULL);

	instance = new Context();
}

void Context::Delete()
{
	SafeDelete(instance);
}

void Context::ResizeScreen()
{
	perspective->Set(D3D::Width(), D3D::Height());
	viewport->Set(D3D::Width(), D3D::Height());
}

Context::Context()
{
	D3DDesc desc = D3D::GetDesc();

	perspective = new Perspective(desc.Width, desc.Height);
	viewport = new Viewport(desc.Width, desc.Height);

	camera = new Freedom();
}

Context::~Context()
{
	SafeDelete(perspective);
	SafeDelete(viewport);
	SafeDelete(camera);
}

void Context::Update()
{
	camera->Update();
}

void Context::Render()
{
	viewport->RSSetViewport();

	string str = string("FPS : ") + to_string(ImGui::GetIO().Framerate);
	Gui::Get()->RenderText(5, 5, 1, 1, 1, str);

	Vector3 cameraPos;
	camera->Position(&cameraPos);

	Vector3 cameraaDir;
	camera->RotationDegree(&cameraaDir);

	str = "Camera(P) : ";
	str += to_string((int)cameraPos.x) + ", " + to_string((int)cameraPos.y) + ", " + to_string((int)cameraPos.z);
	Gui::Get()->RenderText(5, 20, 1, 1, 1, str);


	str = "Camera(R) : ";
	str += to_string((int)cameraaDir.x) + ", " + to_string((int)cameraaDir.y) + ", " + to_string((int)cameraaDir.z);
	Gui::Get()->RenderText(5, 35, 1, 1, 1, str);

}

Matrix Context::View()
{
	Matrix view;
	camera->GetMatrix(&view);

	return view;
}

D3DXMATRIX Context::Projection()
{
	D3DXMATRIX projection;
	perspective->GetMatrix(&projection);

	return projection;
}
