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

		Create(); //자식의 모양을 결정하는 순수 가상합수

				  //CreateBuffer(); 
		vertexBuffer = new VertexBuffer(vertices, vertexCount, sizeof(MeshVertex));
		indexBuffer = new IndexBuffer(indices, indexCount);
	}

	///다 지워버리고
	Super::Update(); // 업데이트 없으니 부모 업데이트로
	Super::Render(); //부모 렌더 호출

	if(diffuseMap != NULL)
		sDiffuseMap->SetResource(diffuseMap->SRV()); //Shader 넘기기

	shader->DrawIndexed(0, Pass(), indexCount); //< pass -> Pass()
}



/// 월드 관련 함수는 다 삭제(포지션, 스케일, 로테이션 등등..)

void Mesh::DiffuseMap(wstring file)
{
	SafeDelete(diffuseMap); //기존 텍스쳐 있으면 지우고

	diffuseMap = new Texture(file);
}