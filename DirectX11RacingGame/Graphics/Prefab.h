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
	// 网格数据
	struct MeshData
	{
		std::vector<Vertex> vertexVec;	// 顶点数组
		std::vector<DWORD> indexVec;	// 索引数组

		MeshData()
		{
			// 需检验索引类型合法性
			static_assert(sizeof(DWORD) == 2 || sizeof(DWORD) == 4, "The size of IndexType must be 2 bytes or 4 bytes!");
			static_assert(std::is_unsigned<DWORD>::value, "IndexType must be unsigned integer!");
		}
	};

	// 创建球体网格数据，levels和slices越大，精度越高。
	static MeshData CreateSphere(float radius = 1.0f, int levels = 20, int slices = 20);

	// 创建立方体网格数据
	static MeshData CreateBox(float width = 2.0f, float height = 2.0f, float depth = 2.0f);

	// 创建圆柱体网格数据，slices越大，精度越高。
	static MeshData CreateCylinder(float radius = 1.0f, float height = 2.0f, int slices = 20);

	// 创建只有圆柱体侧面的网格数据，slices越大，精度越高
	static MeshData CreateCylinderSide(float radius = 1.0f, float height = 2.0f, int slices = 5);

	// 创建一个平面
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
    // 将我们构造的VertexData类型转为Vertex类型
    static void ConvertToVertexType(Vertex& vertexDst, const VertexData& vertexSrc)
    {
        vertexDst.position = vertexSrc.position;
        vertexDst.texCoord = vertexSrc.texCoord;
    }

private:
	static const std::map<std::string, std::pair<size_t, size_t>> semanticSizeMap;
};

// 几何体方法的实现
inline Prefab::MeshData Prefab::CreateSphere(float radius, int levels, int slices)
{
	using namespace DirectX;
	
	//constexpr size_t NumElems = ARRAYSIZE(Vertex::inputLayout);

	MeshData meshData;
	meshData.vertexVec.resize(2 + (levels - 1) * (slices + 1));
	meshData.indexVec.resize(6 * (levels - 1) * slices);

	VertexData vertexData;
	DWORD vIndex = 0, iIndex = 0;
    // 球体是根据两个角度的迭代计算出来的，可以看做是经度和维度
	float phi = 0.0f, theta = 0.0f;
	float per_phi = XM_PI / levels;
	float per_theta = XM_2PI / slices;
	float x, y, z;

	// 放入顶端点
	vertexData = { XMFLOAT3(0.0f, radius, 0.0f), XMFLOAT2(0.0f, 0.0f) };
    ConvertToVertexType(meshData.vertexVec[vIndex], vertexData);

	for (int i = 1; i < levels; ++i)
	{
		phi = per_phi * i;
		// 需要slices + 1个顶点是因为 起点和终点需为同一点，但纹理坐标值不一致
		for (int j = 0; j <= slices; ++j)
		{
			theta = per_theta * j;
			x = radius * sinf(phi) * cosf(theta);   // x和z轴需要先根据纬度拿到所在圆的半径，再确定坐标
			y = radius * cosf(phi);                 // y轴直接cos纬度即可
			z = radius * sinf(phi) * sinf(theta);
			// 计算出局部坐标和纹理坐标
			XMFLOAT3 position = XMFLOAT3(x, y, z);
			vertexData = { position, XMFLOAT2(theta / XM_2PI, phi / XM_PI) };
            ConvertToVertexType(meshData.vertexVec[vIndex], vertexData);
		}
	}
    
	// 放入底端点
	vertexData = { XMFLOAT3(0.0f, -radius, 0.0f), XMFLOAT2(0.0f, 1.0f) };
    ConvertToVertexType(meshData.vertexVec[vIndex], vertexData);

	// 逐渐放入索引
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

	// 逐渐放入索引
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

	VertexData vertexDataArray[24]; // 24个顶点
	float l = length / 2, h = height / 2, w = width / 2;

	// 顶面
	vertexDataArray[0].position = XMFLOAT3(-l, h, -w);
	vertexDataArray[1].position = XMFLOAT3(-l, h, w);
	vertexDataArray[2].position = XMFLOAT3(l, h, w);
	vertexDataArray[3].position = XMFLOAT3(l, h, -w);
	// 底面
	vertexDataArray[4].position = XMFLOAT3(l, -h, -w);
	vertexDataArray[5].position = XMFLOAT3(l, -h, w);
	vertexDataArray[6].position = XMFLOAT3(-l, -h, w);
	vertexDataArray[7].position = XMFLOAT3(-l, -h, -w);
	// 左面
	vertexDataArray[8].position = XMFLOAT3(-l, -h, w);
	vertexDataArray[9].position = XMFLOAT3(-l, h, w);
	vertexDataArray[10].position = XMFLOAT3(-l, h, -w);
	vertexDataArray[11].position = XMFLOAT3(-l, -h, -w);
	// 右面
	vertexDataArray[12].position = XMFLOAT3(l, -h, -w);
	vertexDataArray[13].position = XMFLOAT3(l, h, -w);
	vertexDataArray[14].position = XMFLOAT3(l, h, w);
	vertexDataArray[15].position = XMFLOAT3(l, -h, w);
	// 前面
	vertexDataArray[16].position = XMFLOAT3(l, -h, w);
	vertexDataArray[17].position = XMFLOAT3(l, h, w);
	vertexDataArray[18].position = XMFLOAT3(-l, h, w);
	vertexDataArray[19].position = XMFLOAT3(-l, -h, w);
	// 后面
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
		0, 1, 2, 2, 3, 0,		// 顶面
		4, 5, 6, 6, 7, 4,		// 底面
		8, 9, 10, 10, 11, 8,	// 左面
		12, 13, 14, 14, 15, 12,	// 右面
		16, 17, 18, 18, 19, 16, // 背面
		20, 21, 22, 22, 23, 20	// 正面
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

	// 放入顶端圆心
	vertexData = { XMFLOAT3(0.0f, h2, 0.0f), XMFLOAT2(0.5f, 0.5f) };
	ConvertToVertexType(meshData.vertexVec[vIndex++], vertexData);

	// 放入顶端圆上各点
	for (int i = 0; i <= slices; ++i)
	{
		theta = i * per_theta;
		vertexData = { XMFLOAT3(radius * cosf(theta), h2, radius * sinf(theta)), XMFLOAT2(cosf(theta) / 2 + 0.5f, sinf(theta) / 2 + 0.5f) };
        ConvertToVertexType(meshData.vertexVec[vIndex++], vertexData);
	}

	// 放入底部圆上各点
	for (int i = 0; i <= slices; ++i)
	{
		theta = i * per_theta;
		vertexData = { XMFLOAT3(radius * cosf(theta), -h2, radius * sinf(theta)), XMFLOAT2(cosf(theta) / 2 + 0.5f, sinf(theta) / 2 + 0.5f) };
        ConvertToVertexType(meshData.vertexVec[vIndex++], vertexData);
	}

	// 放入底端圆心
	vertexData = { XMFLOAT3(0.0f, -h2, 0.0f), XMFLOAT2(0.5f, 0.5f) };
    ConvertToVertexType(meshData.vertexVec[vIndex++], vertexData);

	// 逐渐放入顶部三角形索引
	for (int i = 1; i <= slices; ++i)
	{
		meshData.indexVec[iIndex++] = offset;
		meshData.indexVec[iIndex++] = offset + i % (slices + 1) + 1;
		meshData.indexVec[iIndex++] = offset + i;
	}

	// 逐渐放入底部三角形索引
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
    // 初始化
	MeshData meshData;
	meshData.vertexVec.resize(2 * (slices + 1));
	meshData.indexVec.resize(6 * slices);

	size_t iIndex = 0;
    // 这里我们只分两层，即顶层和底层，来绘制侧面
	float h2 = height / 2;
	float theta = 0.0f;
	float deltaTheta = XM_2PI / slices; // 得到每个切面的计算角度

	VertexData vertexData;

	// 放入侧面顶端点（时间不够做第二张贴图，所以就把侧面的uv绘制为纯色）
	for (int i = 0; i <= slices; ++i)
	{
		theta = i * deltaTheta;
		vertexData = { XMFLOAT3(radius * cosf(theta), h2, radius * sinf(theta)), XMFLOAT2(0, 0) };
        ConvertToVertexType(meshData.vertexVec[i], vertexData);
	}

	// 放入侧面底端点
	for (int i = 0; i <= slices; ++i)
	{
		theta = i * deltaTheta;
		vertexData = { XMFLOAT3(radius * cosf(theta), -h2, radius * sinf(theta)), XMFLOAT2(0, 1) };
        ConvertToVertexType(meshData.vertexVec[(slices + 1) + i], vertexData);
	}

	// 放入索引（只有两层，所以一个切面只有两个三角形）
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
    // 平面就画了一个四边形
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
