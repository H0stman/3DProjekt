#include "Terrain.hpp"

void Terrain::ReadFileData(PCSTR filename)
{
	FILE* filePtr;                            // Point to the current position in the file
	BITMAPFILEHEADER bitmapFileHeader;        // Structure which stores information about file
	BITMAPINFOHEADER bitmapInfoHeader;        // Structure which stores information about image
	INT imageSize, index;
	UCHAR height;

	// Open the file
	fopen_s(&filePtr, filename, "rb");

	// Read bitmaps header
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

	// Read the info header
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	// Get the width and height (width and length) of the image
	hminfo.terrainWidth = bitmapInfoHeader.biWidth;
	hminfo.terrainHeight = bitmapInfoHeader.biHeight;

	// Size of the image in bytes. the 3 represents RBG (byte, byte, byte) for each pixel
	imageSize = hminfo.terrainWidth * hminfo.terrainHeight * 3;

	// Initialize the array which stores the image data
	PUCHAR bitmapImage = new UCHAR[imageSize];

	// Set the file pointer to the beginning of the image data
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// Store image data in bitmapImage
	fread(bitmapImage, 1, imageSize, filePtr);

	// Close file
	fclose(filePtr);

	// Initialize the heightMap array (stores the vertices terrain)
	hminfo.heightMap = new XMFLOAT3[hminfo.terrainWidth * hminfo.terrainHeight];

	// Using a greyscale image, so all 3 rgb values are the same, but we only need one for the height
	// So we use this counter to skip the next two components in the image data (we read R, then skip BG)
	INT k = 0;

	// We divide the height by this number to "water down" the terrains height, otherwise the terrain will
	// appear to be "spikey" and not so smooth.
	FLOAT heightFactor = 10.0f;

	// Read the image data into our heightMap array
	for (UINT j = 0; j < hminfo.terrainHeight; j++)
		for (UINT i = 0; i < hminfo.terrainWidth; i++)
		{
			height = bitmapImage[k];

			index = (hminfo.terrainHeight * j) + i;

			hminfo.heightMap[index].x = (FLOAT)i;
			hminfo.heightMap[index].y = (FLOAT)height / heightFactor;
			hminfo.heightMap[index].z = (FLOAT)j;

			k += 3;
		}

	delete[] bitmapImage;
	bitmapImage = 0;
}

void Terrain::CreateVerticesAndIndexList()
{
	INT cols = hminfo.terrainWidth;
	INT rows = hminfo.terrainHeight;

	//Create the grid
	vertexcount = rows * cols;
	facecount = (rows - 1) * (cols - 1) * 2;

	std::vector<vertex> vertices(vertexcount);

	for (DWORD i = 0; i < rows; ++i)
		for (DWORD j = 0; j < cols; ++j)
		{
			vertices[i * cols + j].position = hminfo.heightMap[i * cols + j];
			vertices[i * cols + j].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		}

	std::vector<DWORD> indices(facecount * 3);

	INT k, texUIndex, texVIndex = 0;

	for (DWORD i = 0; i < rows - 1; i++)
	{
		for (DWORD j = 0; j < cols - 1; j++)
		{
			indices[k] = i * cols + j;        // Bottom left of quad
			vertices[i * cols + j].texturecoordinate = XMFLOAT2(texUIndex + 0.0f, texVIndex + 1.0f);

			indices[k + 1] = i * cols + j + 1;        // Bottom right of quad
			vertices[i * cols + j + 1].texturecoordinate = XMFLOAT2(texUIndex + 1.0f, texVIndex + 1.0f);

			indices[k + 2] = (i + 1) * cols + j;    // Top left of quad
			vertices[(i + 1) * cols + j].texturecoordinate = XMFLOAT2(texUIndex + 0.0f, texVIndex + 0.0f);


			indices[k + 3] = (i + 1) * cols + j;    // Top left of quad
			vertices[(i + 1) * cols + j].texturecoordinate = XMFLOAT2(texUIndex + 0.0f, texVIndex + 0.0f);

			indices[k + 4] = i * cols + j + 1;        // Bottom right of quad
			vertices[i * cols + j + 1].texturecoordinate = XMFLOAT2(texUIndex + 1.0f, texVIndex + 1.0f);

			indices[k + 5] = (i + 1) * cols + j + 1;    // Top right of quad
			vertices[(i + 1) * cols + j + 1].texturecoordinate = XMFLOAT2(texUIndex + 1.0f, texVIndex + 0.0f);

			k += 6; // next quad

			texUIndex++;
		}
		texUIndex = 0;
		texVIndex++;
	}
}

Terrain::Terrain(PCSTR filename)
{
	facecount, vertexcount = 0;
	vertices.clear();
	indices.clear();
	ReadFileData(filename);
	CreateVerticesAndIndexList();
}

void Terrain::PrimePipeline(UINT pipelinesettings)
{

	// Create index buffer

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * facecount * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));

	initData.pSysMem = &indices[0];

	HRESULT hr = Engine::GetInstance()->GetDevice()->CreateBuffer(&indexBufferDesc, &initData, &indexbuffer);

	// Create vertex buffer

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(vertex) * vertexcount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = &vertices[0];
	hr = Engine::GetInstance()->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexbuffer);

	// Setup pipeline to appropriate state : TODO



}

