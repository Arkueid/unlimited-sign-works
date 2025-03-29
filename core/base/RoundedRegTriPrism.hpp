#pragma once

#include <base/Mesh.hpp>

/// @brief 建模中心：下底面正三角形中心
class RoundedRegTriPrism : public Mesh
{
	Vertex* vertices;
	int vertexCount;
	int drawableSize[3];
	int drawableOffset[3];

	float a, h, radius;
public:
	/// @brief 正圆角三角形垂直轴心平移得到的几何柱体
	/// @param a 正三角形边长
	/// @param h 高/厚度
	/// @param radius 圆角半径
	/// @param precision 圆角上圆周每单位长度的顶点个数
	RoundedRegTriPrism(float a, float h, float radius, float precision = PRECISION_DEFAULT);
	~RoundedRegTriPrism() override;

	int getType() override
	{
		return MeshType_RoundedRegTriPrism;
	}

	// 通过 Mesh 继承
	Vertex* getVertices() override;

	int getVertexCount() override;

	int getDrawableCount() override;

	int getDrawableOffset(int index) override;

	int getDrawableSize(int index) override;

};