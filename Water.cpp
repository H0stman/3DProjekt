#include "Water.hpp"

Water::Water(ID3D11Device* device) : Model("water.obj", device)
{

}

VOID Water::UpdateWater(ID3D11DeviceContext* context)
{
	D3D11_MAPPED_SUBRESOURCE watervertices;
	HRESULT hr = context->Map(vertexbuffer, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &watervertices);
	vertex* vertices = (vertex*)watervertices.pData;
	for (size_t i = 0; i < 4; i++)
	{
		if (vertices[i].texturecoordinate.x > 1.0f || vertices[i].texturecoordinate.y > 1.0f)
		{
			vertices[i].texturecoordinate.x += 0.0f;
			vertices[i].texturecoordinate.y += 0.0f;
		}
		vertices[i].texturecoordinate.x += 0.001f;
		vertices[i].texturecoordinate.y += 0.001f;
	}
	context->Unmap(vertexbuffer, 0u);
}
