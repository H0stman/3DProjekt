#include "Model.hpp"

Model::Model(std::string file, ID3D11Device* device)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(file.c_str(),
		  aiProcess_Triangulate
		| aiProcess_ConvertToLeftHanded
		| aiProcess_CalcTangentSpace
		| aiProcess_GenNormals
		| aiProcess_JoinIdenticalVertices);
	if (pScene == nullptr)
	{
		OutputDebugString(L"Error retrieving scene from file");
	}
	else
	{
		aiMesh* mesh = pScene->mMeshes[0];

		// Saving the xyz max and min to determine the BoundingBox boundig volume
		DirectX::XMFLOAT3 max = { 0.0, 0.0, 0.0 };
		DirectX::XMFLOAT3 min = { 0.0, 0.0, 0.0 };
		for (unsigned int i{ 0u }; i < mesh->mNumVertices; i++)
		{
			vertex vtx;
			vtx.position.x = mesh->mVertices[i].x;
			vtx.position.y = mesh->mVertices[i].y;
			vtx.position.z = mesh->mVertices[i].z;

			if (max.x < vtx.position.x) max.x = vtx.position.x;
			if (vtx.position.x < min.x) min.x = vtx.position.x;
			if (max.y < vtx.position.y) max.y = vtx.position.y;
			if (vtx.position.y < min.y) min.y = vtx.position.y;
			if (max.z < vtx.position.z) max.z = vtx.position.z;
			if (vtx.position.z < min.z) min.z = vtx.position.z;

			vtx.texturecoordinate.x = mesh->mTextureCoords[0][i].x;
			vtx.texturecoordinate.y = mesh->mTextureCoords[0][i].y;

			vtx.normal.x = mesh->mNormals[i].x;
			vtx.normal.y = mesh->mNormals[i].y;
			vtx.normal.z = mesh->mNormals[i].z;

			if (mesh->HasTangentsAndBitangents())
			{
				vtx.tangent.x = mesh->mTangents[i].x;
				vtx.tangent.y = mesh->mTangents[i].y;
				vtx.tangent.z = mesh->mTangents[i].z;

				vtx.bitangent.x = mesh->mBitangents[i].x;
				vtx.bitangent.y = mesh->mBitangents[i].y;
				vtx.bitangent.z = mesh->mBitangents[i].z;
			}
			vertices.push_back(vtx);
		}
		BoundingBox::CreateFromPoints(boundingbox, XMVectorSet(max.x, max.y, max.z, 1.0f), XMVectorSet(min.x, min.y, min.z, 1.0f));
		//m_NrOfVertices = static_cast<unsigned int>(vertices.size());

		for (unsigned int i{ 0u }; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j{ 0u }; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		for (size_t i = 0; i < 3; ++i)
			texture.push_back(nullptr);

		aiString texPath;
		if (pScene->mMaterials[mesh->mMaterialIndex]->GetTextureCount(aiTextureType_DIFFUSE) != 0)
		{
			pScene->mMaterials[mesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &texPath);
			std::string tex = std::string(texPath.C_Str(), texPath.length);
			texture[diffuse] = new Texture(tex, device);
		}

		texPath.Clear();
		if (pScene->mMaterials[mesh->mMaterialIndex]->GetTextureCount(aiTextureType_DISPLACEMENT) != 0)
		{
			pScene->mMaterials[mesh->mMaterialIndex]->GetTexture(aiTextureType_DISPLACEMENT, 0, &texPath);
			std::string tex = std::string(texPath.C_Str(), texPath.length);
			texture[displacement] = new Texture(tex, device);
		}

		texPath.Clear();
		if (pScene->mMaterials[mesh->mMaterialIndex]->GetTextureCount(aiTextureType_NORMALS) != 0)
		{
			pScene->mMaterials[mesh->mMaterialIndex]->GetTexture(aiTextureType_NORMALS, 0, &texPath);
			std::string tex = std::string(texPath.C_Str(), texPath.length);
			texture[normalmap] = new Texture(tex, device);
		}

		// These lines seem to throw a delete scalar error
		//if(mesh != nullptr) delete mesh;
		//if(pScene != nullptr) delete pScene;

		/*****Vertex buffer description *****/
		D3D11_BUFFER_DESC vertexBufferDescriptor;
		ZeroMemory(&vertexBufferDescriptor, sizeof(D3D11_BUFFER_DESC));

		vertexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;									//CPU has no access and GPU has read only, i.e. buffer won't ever change. Fastest option.
		vertexBufferDescriptor.ByteWidth = static_cast<UINT>(sizeof(vertex) * vertices.size()); //Size in bytes of Vertex buffer.
		vertexBufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;							//The buffer is of type Vertex buffer.
		vertexBufferDescriptor.CPUAccessFlags = 0u;												//CPU does not require read or write access after the buffer has been created.
		vertexBufferDescriptor.MiscFlags = 0u;													//No misc flags needed for Vertex buffer.
		vertexBufferDescriptor.StructureByteStride = 0u;										//Only applies for structured buffers.

		D3D11_SUBRESOURCE_DATA vertexBufferInitData;
		ZeroMemory(&vertexBufferInitData, sizeof(D3D11_SUBRESOURCE_DATA));
		vertexBufferInitData.pSysMem = vertices.data(); // m_Repo->VertexArray();

		/*****Creating Vertex buffer*****/
		HRESULT HR = device->CreateBuffer(&vertexBufferDescriptor, &vertexBufferInitData, &vertexbuffer);
		if (FAILED(HR))
		{
			OutputDebugString(L"Error creating Vertex Buffer for model.");
		}

		/*Index Buffer description*/
		D3D11_BUFFER_DESC indexBufferDescriptor;
		ZeroMemory(&indexBufferDescriptor, sizeof(D3D11_BUFFER_DESC));

		indexBufferDescriptor.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDescriptor.ByteWidth = static_cast<UINT>(sizeof(DWORD) * indices.size());
		indexBufferDescriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDescriptor.CPUAccessFlags = 0u;
		indexBufferDescriptor.MiscFlags = 0u;
		indexBufferDescriptor.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA indexBufferInitData;
		ZeroMemory(&indexBufferInitData, sizeof(D3D11_SUBRESOURCE_DATA));
		indexBufferInitData.pSysMem = indices.data();

		/*Creating Index Buffer*/
		HR = device->CreateBuffer(&indexBufferDescriptor, &indexBufferInitData, &indexbuffer);
		if (FAILED(HR))
		{
			OutputDebugString(L"Error creating Index Buffer for model.");
		}

		worldmatrix = XMMatrixTranslation(32.0, 15.0, 50.0);
		//worldmatrix = XMMatrixIdentity();
		clockwise = false;
	}
}

Model::~Model()
{
}

UINT Model::GetIndexCount()
{
	return static_cast<UINT>(indices.size());
}


UINT Model::GetStartIndexLocation()
{
	return 0u;
}


INT Model::GetBaseVertexLocation()
{
	return 0u;
}


ID3D11Buffer** Model::GetVertexBuffer()
{
	return &vertexbuffer;
}


ID3D11Buffer* Model::GetIndexBuffer()
{
	return indexbuffer;
}

