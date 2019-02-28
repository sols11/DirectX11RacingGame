#ifndef PREFAB_H_
#define PREFAB_H_

#include <vector>
#include <string>
#include <map>
#include "Vertex.h"
#include "VertexBuffer.h"

class Prefab
{
public:
	// ��������
	struct MeshData
	{
		std::vector<Vertex> vertexVec;	// ��������
		std::vector<DWORD> indexVec;	// ��������

		MeshData()
		{
			// ������������ͺϷ���
			static_assert(sizeof(DWORD) == 2 || sizeof(DWORD) == 4, "The size of IndexType must be 2 bytes or 4 bytes!");
			static_assert(std::is_unsigned<DWORD>::value, "IndexType must be unsigned integer!");
		}
	};

	// ���������������ݣ�levels��slicesԽ�󣬾���Խ�ߡ�
	static MeshData CreateSphere(float radius = 1.0f, int levels = 20, int slices = 20);

	// ������������������
	static MeshData CreateBox(float width = 2.0f, float height = 2.0f, float depth = 2.0f);

	// ����Բ�����������ݣ�slicesԽ�󣬾���Խ�ߡ�
	static MeshData CreateCylinder(float radius = 1.0f, float height = 2.0f, int slices = 20);

	// ����ֻ��Բ���������������ݣ�slicesԽ�󣬾���Խ��
	static MeshData CreateCylinderSide(float radius = 1.0f, float height = 2.0f, int slices = 5);

	// ����һ��ƽ��
	static MeshData CreatePlane(const DirectX::XMFLOAT3& center, const DirectX::XMFLOAT2& planeSize = { 10.0f, 10.0f },
		const DirectX::XMFLOAT2& maxTexCoord = { 1.0f, 1.0f });
	static MeshData CreatePlane(float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f,
		float width = 10.0f, float depth = 10.0f, float texU = 1.0f, float texV = 1.0f);
    
private:
	struct VertexData
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texCoord;
	};
    // �����ǹ����VertexData����תΪVertex����
    static void ConvertToVertexType(Vertex& vertexDst, const VertexData& vertexSrc)
    {
        vertexDst.position = vertexSrc.position;
        vertexDst.texCoord = vertexSrc.texCoord;
    }

private:
	static const std::map<std::string, std::pair<size_t, size_t>> semanticSizeMap;
};

// �����巽����ʵ��
inline Prefab::MeshData Prefab::CreateSphere(float radius, int levels, int slices)
{
	using namespace DirectX;
	
	//constexpr size_t NumElems = ARRAYSIZE(Vertex::inputLayout);

	MeshData meshData;
	meshData.vertexVec.resize(2 + (levels - 1) * (slices + 1));
	meshData.indexVec.resize(6 * (levels - 1) * slices);

	VertexData vertexData;
	DWORD vIndex = 0, iIndex = 0;
    // �����Ǹ��������Ƕȵĵ�����������ģ����Կ����Ǿ��Ⱥ�ά��
	float phi = 0.0f, theta = 0.0f;
	float per_phi = XM_PI / levels;
	float per_theta = XM_2PI / slices;
	float x, y, z;

	// ���붥�˵�
	vertexData = { XMFLOAT3(0.0f, radius, 0.0f), XMFLOAT2(0.0f, 0.0f) };
    ConvertToVertexType(meshData.vertexVec[vIndex], vertexData);

	for (int i = 1; i < levels; ++i)
	{
		phi = per_phi * i;
		// ��Ҫslices + 1����������Ϊ �����յ���Ϊͬһ�㣬����������ֵ��һ��
		for (int j = 0; j <= slices; ++j)
		{
			theta = per_theta * j;
			x = radius * sinf(phi) * cosf(theta);   // x��z����Ҫ�ȸ���γ���õ�����Բ�İ뾶����ȷ������
			y = radius * cosf(phi);                 // y��ֱ��cosγ�ȼ���
			z = radius * sinf(phi) * sinf(theta);
			// ������ֲ��������������
			XMFLOAT3 position = XMFLOAT3(x, y, z);
			vertexData = { position, XMFLOAT2(theta / XM_2PI, phi / XM_PI) };
            ConvertToVertexType(meshData.vertexVec[vIndex], vertexData);
		}
	}
    
	// ����׶˵�
	vertexData = { XMFLOAT3(0.0f, -radius, 0.0f), XMFLOAT2(0.0f, 1.0f) };
    ConvertToVertexType(meshData.vertexVec[vIndex], vertexData);

	// �𽥷�������
	if (levels > 1)
	{
		for (int j = 1; j <= slices; ++j)
		{
			meshData.indexVec[iIndex++] = 0;
			meshData.indexVec[iIndex++] = j % (slices + 1) + 1;
			meshData.indexVec[iIndex++] = j;
		}
	}


	for (int i = 1; i < levels - 1; ++i)
	{
		for (int j = 1; j <= slices; ++j)
		{
			meshData.indexVec[iIndex++] = (i - 1) * (slices + 1) + j;
			meshData.indexVec[iIndex++] = (i - 1) * (slices + 1) + j % (slices + 1) + 1;
			meshData.indexVec[iIndex++] = i * (slices + 1) + j % (slices + 1) + 1;

			meshData.indexVec[iIndex++] = i * (slices + 1) + j % (slices + 1) + 1;
			meshData.indexVec[iIndex++] = i * (slices + 1) + j;
			meshData.indexVec[iIndex++] = (i - 1) * (slices + 1) + j;
		}
	}

	// �𽥷�������
	if (levels > 1)
	{
		for (int j = 1; j <= slices; ++j)
		{
			meshData.indexVec[iIndex++] = (levels - 2) * (slices + 1) + j;
			meshData.indexVec[iIndex++] = (levels - 2) * (slices + 1) + j % (slices + 1) + 1;
			meshData.indexVec[iIndex++] = (levels - 1) * (slices + 1) + 1;
		}
	}

	return meshData;
}

inline Prefab::MeshData Prefab::CreateBox(float length, float height, float width)
{
	using namespace DirectX;

	MeshData meshData;
	meshData.vertexVec.resize(24);

	VertexData vertexDataArray[24]; // 24������
	float l = length / 2, h = height / 2, w = width / 2;

	// ����
	vertexDataArray[0].position = XMFLOAT3(-l, h, -w);
	vertexDataArray[1].position = XMFLOAT3(-l, h, w);
	vertexDataArray[2].position = XMFLOAT3(l, h, w);
	vertexDataArray[3].position = XMFLOAT3(l, h, -w);
	// ����
	vertexDataArray[4].position = XMFLOAT3(l, -h, -w);
	vertexDataArray[5].position = XMFLOAT3(l, -h, w);
	vertexDataArray[6].position = XMFLOAT3(-l, -h, w);
	vertexDataArray[7].position = XMFLOAT3(-l, -h, -w);
	// ����
	vertexDataArray[8].position = XMFLOAT3(-l, -h, w);
	vertexDataArray[9].position = XMFLOAT3(-l, h, w);
	vertexDataArray[10].position = XMFLOAT3(-l, h, -w);
	vertexDataArray[11].position = XMFLOAT3(-l, -h, -w);
	// ����
	vertexDataArray[12].position = XMFLOAT3(l, -h, -w);
	vertexDataArray[13].position = XMFLOAT3(l, h, -w);
	vertexDataArray[14].position = XMFLOAT3(l, h, w);
	vertexDataArray[15].position = XMFLOAT3(l, -h, w);
	// ǰ��
	vertexDataArray[16].position = XMFLOAT3(l, -h, w);
	vertexDataArray[17].position = XMFLOAT3(l, h, w);
	vertexDataArray[18].position = XMFLOAT3(-l, h, w);
	vertexDataArray[19].position = XMFLOAT3(-l, -h, w);
	// ����
	vertexDataArray[20].position = XMFLOAT3(-l, -h, -w);
	vertexDataArray[21].position = XMFLOAT3(-l, h, -w);
	vertexDataArray[22].position = XMFLOAT3(l, h, -w);
	vertexDataArray[23].position = XMFLOAT3(l, -h, -w);

	for (int i = 0; i < 6; ++i)
	{
		vertexDataArray[i * 4].texCoord = XMFLOAT2(0.0f, 1.0f);
		vertexDataArray[i * 4 + 1].texCoord = XMFLOAT2(0.0f, 0.0f);
		vertexDataArray[i * 4 + 2].texCoord = XMFLOAT2(1.0f, 0.0f);
		vertexDataArray[i * 4 + 3].texCoord = XMFLOAT2(1.0f, 1.0f);
	}

	for (int i = 0; i < 24; ++i)
	{
		ConvertToVertexType(meshData.vertexVec[i], vertexDataArray[i]);
	}

	meshData.indexVec = {
		0, 1, 2, 2, 3, 0,		// ����
		4, 5, 6, 6, 7, 4,		// ����
		8, 9, 10, 10, 11, 8,	// ����
		12, 13, 14, 14, 15, 12,	// ����
		16, 17, 18, 18, 19, 16, // ����
		20, 21, 22, 22, 23, 20	// ����
	};

	return meshData;
}

inline Prefab::MeshData Prefab::CreateCylinder(float radius, float height, int slices)    
{
	using namespace DirectX;

	auto meshData = CreateCylinderSide(radius, height, slices);
	meshData.vertexVec.resize(4 * (slices + 1) + 2);
	meshData.indexVec.resize(12 * slices);

	float h2 = height / 2;
	float theta = 0.0f;
	float per_theta = XM_2PI / slices;

	DWORD vIndex = 2 * (slices + 1), iIndex = 6 * slices;
	DWORD offset = 2 * (slices + 1);
	VertexData vertexData;

	// ���붥��Բ��
	vertexData = { XMFLOAT3(0.0f, h2, 0.0f), XMFLOAT2(0.5f, 0.5f) };
	ConvertToVertexType(meshData.vertexVec[vIndex++], vertexData);

	// ���붥��Բ�ϸ���
	for (int i = 0; i <= slices; ++i)
	{
		theta = i * per_theta;
		vertexData = { XMFLOAT3(radius * cosf(theta), h2, radius * sinf(theta)), XMFLOAT2(cosf(theta) / 2 + 0.5f, sinf(theta) / 2 + 0.5f) };
        ConvertToVertexType(meshData.vertexVec[vIndex++], vertexData);
	}

	// ����ײ�Բ�ϸ���
	for (int i = 0; i <= slices; ++i)
	{
		theta = i * per_theta;
		vertexData = { XMFLOAT3(radius * cosf(theta), -h2, radius * sinf(theta)), XMFLOAT2(cosf(theta) / 2 + 0.5f, sinf(theta) / 2 + 0.5f) };
        ConvertToVertexType(meshData.vertexVec[vIndex++], vertexData);
	}

	// ����׶�Բ��
	vertexData = { XMFLOAT3(0.0f, -h2, 0.0f), XMFLOAT2(0.5f, 0.5f) };
    ConvertToVertexType(meshData.vertexVec[vIndex++], vertexData);

	// �𽥷��붥������������
	for (int i = 1; i <= slices; ++i)
	{
		meshData.indexVec[iIndex++] = offset;
		meshData.indexVec[iIndex++] = offset + i % (slices + 1) + 1;
		meshData.indexVec[iIndex++] = offset + i;
	}

	// �𽥷���ײ�����������
	offset += slices + 2;
	for (int i = 1; i <= slices; ++i) 
	{
		meshData.indexVec[iIndex++] = offset;
		meshData.indexVec[iIndex++] = offset + i;
		meshData.indexVec[iIndex++] = offset + i % (slices + 1) + 1;
	}

	return meshData;
}

inline Prefab::MeshData Prefab::CreateCylinderSide(float radius, float height, int slices)
{
	using namespace DirectX;
    // ��ʼ��
	MeshData meshData;
	meshData.vertexVec.resize(2 * (slices + 1));
	meshData.indexVec.resize(6 * slices);

	size_t iIndex = 0;
    // ��������ֻ�����㣬������͵ײ㣬�����Ʋ���
	float h2 = height / 2;
	float theta = 0.0f;
	float deltaTheta = XM_2PI / slices; // �õ�ÿ������ļ���Ƕ�

	VertexData vertexData;

	// ������涥�˵㣨ʱ�䲻�����ڶ�����ͼ�����ԾͰѲ����uv����Ϊ��ɫ��
	for (int i = 0; i <= slices; ++i)
	{
		theta = i * deltaTheta;
		vertexData = { XMFLOAT3(radius * cosf(theta), h2, radius * sinf(theta)), XMFLOAT2(0, 0) };
        ConvertToVertexType(meshData.vertexVec[i], vertexData);
	}

	// �������׶˵�
	for (int i = 0; i <= slices; ++i)
	{
		theta = i * deltaTheta;
		vertexData = { XMFLOAT3(radius * cosf(theta), -h2, radius * sinf(theta)), XMFLOAT2(0, 1) };
        ConvertToVertexType(meshData.vertexVec[(slices + 1) + i], vertexData);
	}

	// ����������ֻ�����㣬����һ������ֻ�����������Σ�
	for (int i = 0; i < slices; ++i)
	{
		meshData.indexVec[iIndex++] = i;
		meshData.indexVec[iIndex++] = i + 1;
		meshData.indexVec[iIndex++] = (slices + 1) + i + 1;

		meshData.indexVec[iIndex++] = (slices + 1) + i + 1;
		meshData.indexVec[iIndex++] = (slices + 1) + i;
		meshData.indexVec[iIndex++] = i;
	}

	return meshData;
}

inline Prefab::MeshData Prefab::CreatePlane(const DirectX::XMFLOAT3 & center, const DirectX::XMFLOAT2 & planeSize, const DirectX::XMFLOAT2 & maxTexCoord)
{
	return CreatePlane(center.x, center.y, center.z, planeSize.x, planeSize.y, maxTexCoord.x, maxTexCoord.y);
}

inline Prefab::MeshData Prefab::CreatePlane(float centerX, float centerY, float centerZ, float width, float depth, float texU, float texV)
{
	using namespace DirectX;

	MeshData meshData;
	meshData.vertexVec.resize(4);

	VertexData vertexData;
	DWORD vIndex = 0;
    // ƽ��ͻ���һ���ı���
	vertexData = { XMFLOAT3(centerX - width / 2, centerY, centerZ - depth / 2), XMFLOAT2(0.0f, texV) };
    ConvertToVertexType(meshData.vertexVec[vIndex++], vertexData);

	vertexData = { XMFLOAT3(centerX - width / 2, centerY, centerZ + depth / 2), XMFLOAT2(0.0f, 0.0f) };
    ConvertToVertexType(meshData.vertexVec[vIndex++], vertexData);

	vertexData = { XMFLOAT3(centerX + width / 2, centerY, centerZ + depth / 2), XMFLOAT2(texU, 0.0f) };
    ConvertToVertexType(meshData.vertexVec[vIndex++], vertexData);

	vertexData = { XMFLOAT3(centerX + width / 2, centerY, centerZ - depth / 2), XMFLOAT2(texU, texV) };
	ConvertToVertexType(meshData.vertexVec[vIndex++], vertexData);

	meshData.indexVec = { 0, 1, 2, 2, 3, 0 };
	return meshData;
}

#endif
