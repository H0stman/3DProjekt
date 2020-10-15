#include "Model.hpp"

Model::Model(ID3D11Device* device, std::string file)
{
	//Assimp::Importer importer;

	//const aiScene* pScene = importer.ReadFile(("Data\\" + file).c_str(),
	//	aiProcess_Triangulate
	//	| aiProcess_ConvertToLeftHanded
	//	| aiProcess_CalcTangentSpace
	//	| aiProcess_GenNormals
	//	| aiProcess_JoinIdenticalVertices);
	//if (pScene == nullptr)
	//{
	//	OutputDebugString(L"Error retrieving scene from file");
	//}
	//else
	//{
	//	aiMesh* mesh = pScene->mMeshes[0];

	//	// Saving the xyz max and min to capture the AABB boundig volume
	//	DirectX::XMFLOAT3 max = { 0.0, 0.0, 0.0 };
	//	DirectX::XMFLOAT3 min = { 0.0, 0.0, 0.0 };
	//	for (unsigned int i{ 0u }; i < mesh->mNumVertices; i++)
	//	{
	//		vertex vtx;
	//		vtx.position.x = mesh->mVertices[i].x;
	//		vtx.position.y = mesh->mVertices[i].y;
	//		vtx.position.z = mesh->mVertices[i].z;

	//		if (max.x < vtx.position.x) max.x = vtx.position.x;
	//		if (vtx.position.x < min.x) min.x = vtx.position.x;
	//		if (max.y < vtx.position.y) max.y = vtx.position.y;
	//		if (vtx.position.y < min.y) min.y = vtx.position.y;
	//		if (max.z < vtx.position.z) max.z = vtx.position.z;
	//		if (vtx.position.z < min.z) min.z = vtx.position.z;

	//		vtx.texturecoordinate.x = mesh->mTextureCoords[0][i].x;
	//		vtx.texturecoordinate.y = mesh->mTextureCoords[0][i].y;

	//		vtx.normal.x = mesh->mNormals[i].x;
	//		vtx.normal.y = mesh->mNormals[i].y;
	//		vtx.normal.z = mesh->mNormals[i].z;

	//		if (mesh->HasTangentsAndBitangents())
	//		{
	//			vtx.tangent.x = mesh->mTangents[i].x;
	//			vtx.tangent.y = mesh->mTangents[i].y;
	//			vtx.tangent.z = mesh->mTangents[i].z;

	//			vtx.bitangent.x = mesh->mbitangents[i].x;
	//			vtx.bitangent.y = mesh->mbitangents[i].y;
	//			vtx.bitangent.z = mesh->mbitangents[i].z;
	//		}
	//		vertices.push_back(vtx);
	//	}
	//	boundingbox.CreateFromPoints(boundingbox, 2, { max, min }, sizeof(XMFLOAT3));
	//	m_NrOfVertices = static_cast<unsigned int>(vertices.size());

	//	std::vector<DWORD> indices;
	//	for (unsigned int i{ 0u }; i < mesh->mNumFaces; i++)
	//	{
	//		aiFace face = mesh->mFaces[i];
	//		for (unsigned int j{ 0u }; j < face.mNumIndices; j++)
	//		{
	//			indices.push_back(face.mIndices[j]);
	//		}
	//	}
	//	m_NrOfIndices = static_cast<unsigned int>(indices.size());

	//	aiString texPath;
	//	if (pScene->mMaterials[mesh->mMaterialIndex]->GetTextureCount(aiTextureType_DIFFUSE) == 0)
	//	{
	//		texPath.Append("cubeTex.png");	// Setting a default texture where model has none (does not seem to produce result, probably due to 0 tex-coords)
	//	}
	//	else
	//	{
	//		pScene->mMaterials[mesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &texPath);
	//	}
	//	std::string tex = "Data\\" + std::string(texPath.C_Str(), texPath.length);	// Combining texture file name with path to data folder
	//	m_TexDiffuse.LoadTexture(device, tex);

	//	texPath.Clear();
	//	if (pScene->mMaterials[mesh->mMaterialIndex]->GetTextureCount(aiTextureType_DISPLACEMENT) == 0)
	//	{
	//		m_HasDisplacementMap = false;
	//	}
	//	else
	//	{
	//		pScene->mMaterials[mesh->mMaterialIndex]->GetTexture(aiTextureType_DISPLACEMENT, 0, &texPath);
	//		tex.clear();
	//		tex = "Data\\" + std::string(texPath.C_Str(), texPath.length);	// Combining texture file name with path to data folder
	//		m_TexDisplacement.LoadTexture(device, tex);
	//		m_HasDisplacementMap = true;
	//	}

	//	texPath.Clear();
	//	if (pScene->mMaterials[mesh->mMaterialIndex]->GetTextureCount(aiTextureType_NORMALS) == 0)
	//	{
	//		m_HasNormalMap = false;
	//	}
	//	else
	//	{
	//		pScene->mMaterials[mesh->mMaterialIndex]->GetTexture(aiTextureType_NORMALS, 0, &texPath);
	//		tex.clear();
	//		tex = "Data\\" + std::string(texPath.C_Str(), texPath.length);	// Combining texture file name with path to data folder
	//		m_TexNormalMap.LoadTexture(device, tex);
	//		m_HasNormalMap = true;
	//	}

	//	/*****Vertex buffer description *****/
	//	D3D11_BUFFER_DESC vertexBufferDescriptor;
	//	ZeroMemory(&vertexBufferDescriptor, sizeof(D3D11_BUFFER_DESC));

	//	vertexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;								//CPU has no access and GPU has read only, i.e. buffer won't ever change. Fastest option.
	//	vertexBufferDescriptor.ByteWidth = sizeof(Vertex) * m_NrOfVertices; 			    //Size in bytes of Vertex buffer.
	//	vertexBufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;						//The buffer is of type Vertex buffer.
	//	vertexBufferDescriptor.CPUAccessFlags = 0u;											//CPU does not require read or write access after the buffer has been created.
	//	vertexBufferDescriptor.MiscFlags = 0u;												//No misc flags needed for Vertex buffer.
	//	vertexBufferDescriptor.StructureByteStride = 0u;									//Only applies for structured buffers.

	//	D3D11_SUBRESOURCE_DATA vertexBufferInitData;
	//	ZeroMemory(&vertexBufferInitData, sizeof(D3D11_SUBRESOURCE_DATA));
	//	vertexBufferInitData.pSysMem = vertices.data(); // m_Repo->VertexArray();

	//	/*****Creating Vertex buffer*****/
	//	HRESULT HR = device->CreateBuffer(&vertexBufferDescriptor, &vertexBufferInitData, &m_VertexBuffer);
	//	if (FAILED(HR))
	//	{
	//		MessageBox(nullptr, L"Error creating Vertex Buffer for model.", L"ERROR", MB_OK);
	//	}

	//	/*Index Buffer description*/
	//	D3D11_BUFFER_DESC indexBufferDescriptor;
	//	ZeroMemory(&indexBufferDescriptor, sizeof(D3D11_BUFFER_DESC));

	//	indexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
	//	indexBufferDescriptor.ByteWidth = sizeof(DWORD) * m_NrOfIndices;
	//	indexBufferDescriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//	indexBufferDescriptor.CPUAccessFlags = 0u;
	//	indexBufferDescriptor.MiscFlags = 0u;
	//	indexBufferDescriptor.StructureByteStride = 0u;

	//	D3D11_SUBRESOURCE_DATA indexBufferInitData;
	//	ZeroMemory(&indexBufferInitData, sizeof(D3D11_SUBRESOURCE_DATA));
	//	indexBufferInitData.pSysMem = indices.data();

	//	/*Creating Index Buffer*/
	//	HR = device->CreateBuffer(&indexBufferDescriptor, &indexBufferInitData, &m_IndexBuffer);
	//	if (FAILED(HR))
	//	{
	//		MessageBox(nullptr, L"Error creating Index Buffer for model.", L"ERROR", MB_OK);
	//	}

	//	}
}