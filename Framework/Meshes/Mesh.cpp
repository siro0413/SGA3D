#include "Framework.h"
#include "Mesh.h"

Mesh::Mesh(Shader * shader)
	: Renderer(shader)
{
	/*D3DXMatrixIdentity(&world);

	sWorld = shader->AsMatrix("World");
	sView = shader->AsMatrix("View");
	sProjection = shader->AsMatrix("Projection");

	sColor = shader->AsVector("Color");*/

	sDiffuseMap = shader->AsSRV("DiffuseMap");
}

Mesh::~Mesh()
{
	SafeDeleteArray(vertices);
	SafeDeleteArray(indices);

	/*SafeRelease(vertexBuffer);
	SafeRelease(indexBuffer);*/
}

void Mesh::Render()
{
	if (vertexBuffer == NULL || indexBuffer == NULL)
	{

		Create(); //�ڽ��� ����� �����ϴ� ���� �����ռ�

				  //CreateBuffer(); 
		vertexBuffer = new VertexBuffer(vertices, vertexCount, sizeof(MeshVertex));
		indexBuffer = new IndexBuffer(indices, indexCount);
	}

	///�� ����������
	Super::Update(); // ������Ʈ ������ �θ� ������Ʈ��
	Super::Render(); //�θ� ���� ȣ��

	if(diffuseMap != NULL)
		sDiffuseMap->SetResource(diffuseMap->SRV()); //Shader �ѱ��

	shader->DrawIndexed(0, Pass(), indexCount); //< pass -> Pass()
}



/// ���� ���� �Լ��� �� ����(������, ������, �����̼� ���..)

void Mesh::DiffuseMap(wstring file)
{
	SafeDelete(diffuseMap); //���� �ؽ��� ������ �����

	diffuseMap = new Texture(file);
}