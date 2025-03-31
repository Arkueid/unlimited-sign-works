#include <base/RoundedRegTriPrism.hpp>
#include <algorithm>

RoundedRegTriPrism::RoundedRegTriPrism(float a, float h, float radius, float precision) : a(a), h(h), radius(radius)
{
	// 1/3 圆角周长
	const float perimeter_1_3 = 2 * PI / 3 * radius;
	// 圆角上离散点个数
	const int numOnCorner = static_cast<int>(perimeter_1_3 * precision);
	// 根号三
	const float sqrtOf3 = sqrtf(3);
	// 圆角相切直线段边长
	const float innerA = a - sqrtOf3 * radius * 2;

	vertexCount = numOnCorner * 3 * 4 * 3 * 2 + 3 * 2 + 3 * 2 * 6 + 3 * 6;

	vertices = new Vertex[vertexCount];

	// 6个圆角的圆心；顺序：上底面的上，右下，左下->下底面
	Point centers[6];
	centers[0] = {0.0f, h, -(a / sqrtOf3 - radius * 2)};
	centers[1] = {a / 2 - radius * sqrtOf3, h, a / 2 / sqrtOf3 - radius};
	centers[2] = {-centers[1].x, h, centers[1].z};
	centers[3] = {-centers[0].x, 0, centers[0].z};
	centers[4] = {-centers[1].x, 0, centers[1].z};
	centers[5] = {-centers[2].x, 0, centers[2].z};

	int vertexIndex = 0;

	// 上底面
	drawableOffset[0] = vertexIndex;
	float totalLengthX = radius * 2 + innerA;
	float totalLengthZ = a / 2 / sqrtOf3 + (-centers[0].z) + radius;
	float angle = 210.0f / 180.0f * PI;
	const float angleStep = 2.0f * PI / 3.0f / numOnCorner;
	int circleOffsets[6];
	float a_2r3 = a / 2.0f / sqrtOf3;
	// 圆角
	for (int i = 0; i < 3; i++)
	{
		circleOffsets[i * 2] = vertexIndex;
		for (int j = 0; j < numOnCorner; j++)
		{
			vertices[vertexIndex].x = centers[i].x + radius * cosf(angle);
			vertices[vertexIndex].y = h;
			vertices[vertexIndex].z = centers[i].z + radius * sinf(angle);
			vertices[vertexIndex].nx = 0;
			vertices[vertexIndex].ny = 1;
			vertices[vertexIndex].nz = 0;
			vertices[vertexIndex].u = vertices[vertexIndex].x / totalLengthX + 0.50f;
			vertices[vertexIndex].v = (-vertices[vertexIndex].z + a_2r3) / totalLengthZ;
			vertexIndex++;
			vertices[vertexIndex].x = centers[i].x + radius * cosf(angle + angleStep);
			vertices[vertexIndex].y = h;
			vertices[vertexIndex].z = centers[i].z + radius * sinf(angle + angleStep);
			vertices[vertexIndex].nx = 0;
			vertices[vertexIndex].ny = 1;
			vertices[vertexIndex].nz = 0;
			vertices[vertexIndex].u = vertices[vertexIndex].x / totalLengthX + 0.50f;
			vertices[vertexIndex].v = (-vertices[vertexIndex].z + a_2r3) / totalLengthZ;
			vertexIndex++;
			vertices[vertexIndex].x = centers[i].x;
			vertices[vertexIndex].y = h;
			vertices[vertexIndex].z = centers[i].z;
			vertices[vertexIndex].nx = 0;
			vertices[vertexIndex].ny = 1;
			vertices[vertexIndex].nz = 0;
			vertices[vertexIndex].u = vertices[vertexIndex].x / totalLengthX + 0.50f;
			vertices[vertexIndex].v = (-vertices[vertexIndex].z + a_2r3) / totalLengthZ;
			vertexIndex++;
			angle += angleStep;
		}
		circleOffsets[i * 2 + 1] = vertexIndex - 2;
	}
	// 中心大三角
	vertices[vertexIndex++] = {centers[0].x, h, centers[0].z, 0, 1, 0, centers[0].x / totalLengthX + 0.50f, (-centers[0].z + a_2r3) / totalLengthZ};
	vertices[vertexIndex++] = {centers[1].x, h, centers[1].z, 0, 1, 0, centers[1].x / totalLengthX + 0.50f, (-centers[1].z + a_2r3) / totalLengthZ};
	vertices[vertexIndex++] = {centers[2].x, h, centers[2].z, 0, 1, 0, centers[2].x / totalLengthX + 0.50f, (-centers[2].z + a_2r3) / totalLengthZ};
	// 右四边形
	vertices[vertexIndex++] = {centers[0].x, h, centers[0].z, 0, 1, 0, centers[0].x / totalLengthX + 0.50f, (-centers[0].z + a_2r3) / totalLengthZ};
	vertices[vertexIndex++] = {centers[1].x, h, centers[1].z, 0, 1, 0, centers[1].x / totalLengthX + 0.50f, (-centers[1].z + a_2r3) / totalLengthZ};
	Vertex *v = &vertices[circleOffsets[1]];
	vertices[vertexIndex++] = *v;
	vertices[vertexIndex++] = *v;
	v = &vertices[circleOffsets[2]];
	vertices[vertexIndex++] = *v;
	vertices[vertexIndex++] = {centers[1].x, h, centers[1].z, 0, 1, 0, centers[1].x / totalLengthX + 0.50f, (-centers[1].z + a_2r3) / totalLengthZ};
	// 下四边形
	v = &vertices[circleOffsets[3]];
	vertices[vertexIndex++] = {centers[1].x, h, centers[1].z, 0, 1, 0, centers[1].x / totalLengthX + 0.50f, (-centers[1].z + a_2r3) / totalLengthZ};
	vertices[vertexIndex++] = *v;
	vertices[vertexIndex++] = {centers[2].x, h, centers[2].z, 0, 1, 0, centers[2].x / totalLengthX + 0.50f, (-centers[2].z + a_2r3) / totalLengthZ};
	vertices[vertexIndex++] = *v;
	v = &vertices[circleOffsets[4]];
	vertices[vertexIndex++] = *v;
	vertices[vertexIndex++] = {centers[2].x, h, centers[2].z, 0, 1, 0, centers[2].x / totalLengthX + 0.50f, (-centers[2].z + a_2r3) / totalLengthZ};
	// 左四边形
	v = &vertices[circleOffsets[5]];
	vertices[vertexIndex++] = *v;
	vertices[vertexIndex++] = {centers[2].x, h, centers[2].z, 0, 1, 0, centers[2].x / totalLengthX + 0.50f, (-centers[2].z + a_2r3) / totalLengthZ};
	vertices[vertexIndex++] = {centers[0].x, h, centers[0].z, 0, 1, 0, centers[0].x / totalLengthX + 0.50f, (-centers[0].z + a_2r3) / totalLengthZ};
	vertices[vertexIndex++] = *v;
	v = &vertices[circleOffsets[0]];
	vertices[vertexIndex++] = {centers[0].x, h, centers[0].z, 0, 1, 0, centers[0].x / totalLengthX + 0.50f, (-centers[0].z + a_2r3) / totalLengthZ};
	vertices[vertexIndex++] = *v;
	drawableSize[0] = vertexIndex;

	// 下底面
	drawableOffset[1] = vertexIndex;
	for (int i = 0; i < drawableSize[0]; i++)
	{
		vertices[vertexIndex] = vertices[i];
		vertices[vertexIndex].y = 0;
		vertices[vertexIndex].ny = -1;
		vertexIndex++;
	}
	drawableSize[1] = vertexIndex - drawableOffset[1];

	// 侧面
	float u = 0.0f;
	angle = 210.0f / 180.0f * PI;
	const float totalPerimeter = perimeter_1_3 * 3 + innerA * 3;
	float uStage[3] = {0.0f, (perimeter_1_3 + innerA) / totalPerimeter, (perimeter_1_3 * 2 + innerA * 2) / totalPerimeter};
	const float uStep = perimeter_1_3 / totalPerimeter / numOnCorner;
	drawableOffset[2] = vertexIndex;
	for (int i = 0; i < 3; i++)
	{
		u = uStage[i];
		for (int j = 0; j < numOnCorner; j++)
		{
			// 上三角
			vertices[vertexIndex].x = cosf(angle) * radius + centers[i].x;
			vertices[vertexIndex].y = h;
			vertices[vertexIndex].z = sinf(angle) * radius + centers[i].z;
			vertices[vertexIndex].nx = cosf(angle);
			vertices[vertexIndex].ny = 0;
			vertices[vertexIndex].nz = sinf(angle);
			vertices[vertexIndex].u = u;
			vertices[vertexIndex].v = 1;
			vertexIndex++;
			vertices[vertexIndex].x = cosf(angle + angleStep) * radius + centers[i].x;
			vertices[vertexIndex].y = h;
			vertices[vertexIndex].z = sinf(angle + angleStep) * radius + centers[i].z;
			vertices[vertexIndex].nx = cosf(angle + angleStep);
			vertices[vertexIndex].ny = 0;
			vertices[vertexIndex].nz = sinf(angle + angleStep);
			vertices[vertexIndex].u = u + uStep;
			vertices[vertexIndex].v = 1;
			vertexIndex++;
			vertices[vertexIndex].x = cosf(angle) * radius + centers[i].x;
			vertices[vertexIndex].y = 0;
			vertices[vertexIndex].z = sinf(angle) * radius + centers[i].z;
			vertices[vertexIndex].nx = cosf(angle);
			vertices[vertexIndex].ny = 0;
			vertices[vertexIndex].nz = sinf(angle);
			vertices[vertexIndex].u = u;
			vertices[vertexIndex].v = 0;
			vertexIndex++;
			// 下三角
			vertices[vertexIndex].x = cosf(angle + angleStep) * radius + centers[i].x;
			vertices[vertexIndex].y = 0;
			vertices[vertexIndex].z = sinf(angle + angleStep) * radius + centers[i].z;
			vertices[vertexIndex].nx = cosf(angle + angleStep);
			vertices[vertexIndex].ny = 0;
			vertices[vertexIndex].nz = sinf(angle + angleStep);
			vertices[vertexIndex].u = u + uStep;
			vertices[vertexIndex].v = 0;
			vertexIndex++;
			vertices[vertexIndex].x = cosf(angle) * radius + centers[i].x;
			vertices[vertexIndex].y = 0;
			vertices[vertexIndex].z = sinf(angle) * radius + centers[i].z;
			vertices[vertexIndex].nx = cosf(angle);
			vertices[vertexIndex].ny = 0;
			vertices[vertexIndex].nz = sinf(angle);
			vertices[vertexIndex].u = u;
			vertices[vertexIndex].v = 0;
			vertexIndex++;
			vertices[vertexIndex].x = cosf(angle + angleStep) * radius + centers[i].x;
			vertices[vertexIndex].y = h;
			vertices[vertexIndex].z = sinf(angle + angleStep) * radius + centers[i].z;
			vertices[vertexIndex].nx = cosf(angle + angleStep);
			vertices[vertexIndex].ny = 0;
			vertices[vertexIndex].nz = sinf(angle + angleStep);
			vertices[vertexIndex].u = u + uStep;
			vertices[vertexIndex].v = 1;
			vertexIndex++;
			u += uStep;
			angle += angleStep;
		}
	}
	float uStage2[3] = {perimeter_1_3 / totalPerimeter, (perimeter_1_3 * 2 + innerA) / totalPerimeter, (perimeter_1_3 * 3 + innerA * 2) / totalPerimeter};
	// 正面
	v = &vertices[circleOffsets[4]];
	vertices[vertexIndex++] = {v->x, h, v->z, 0, 0, 1, uStage[2], 1};
	vertices[vertexIndex++] = {v->x, 0, v->z, 0, 0, 1, uStage[2], 0};
	Vertex *v2 = &vertices[circleOffsets[3]];
	vertices[vertexIndex++] = {v2->x, h, v2->z, 0, 0, 1, uStage2[1], 1};
	vertices[vertexIndex++] = {v2->x, h, v2->z, 0, 0, 1, uStage2[1], 1};
	vertices[vertexIndex++] = {v->x, 0, v->z, 0, 0, 1, uStage[2], 0};
	vertices[vertexIndex++] = {v2->x, 0, v2->z, 0, 0, 1, uStage2[1], 1};

	// 右侧面
	v = &vertices[circleOffsets[2]];
	v2 = &vertices[circleOffsets[1]];
	vertices[vertexIndex++] = {v->x, h, v->z, (v->x + v2->x) / 2, 0, (v->z + v2->z) / 2, uStage[1], 1};
	vertices[vertexIndex++] = {v->x, 0, v->z, (v->x + v2->x) / 2, 0, (v->z + v2->z) / 2, uStage[1], 0};
	vertices[vertexIndex++] = {v2->x, h, v2->z, (v->x + v2->x) / 2, 0, (v->z + v2->z) / 2, uStage2[0], 1};
	vertices[vertexIndex++] = {v2->x, h, v2->z, (v->x + v2->x) / 2, 0, (v->z + v2->z) / 2, uStage2[0], 1};
	vertices[vertexIndex++] = {v2->x, 0, v2->z, (v->x + v2->x) / 2, 0, (v->z + v2->z) / 2, uStage2[0], 1};
	vertices[vertexIndex++] = {v->x, 0, v->z, (v->x + v2->x) / 2, 0, (v->z + v2->z) / 2, uStage[1], 0};

	// 左侧面
	v = &vertices[circleOffsets[0]];
	v2 = &vertices[circleOffsets[5]];
	vertices[vertexIndex++] = {v->x, h, v->z, (v->x + v2->x) / 2, 0, (v->z + v2->z) / 2, uStage[0], 1};
	vertices[vertexIndex++] = {v->x, 0, v->z, (v->x + v2->x) / 2, 0, (v->z + v2->z) / 2, uStage[0], 0};
	vertices[vertexIndex++] = {v2->x, h, v2->z, (v->x + v2->x) / 2, 0, (v->z + v2->z) / 2, uStage2[2], 1};
	vertices[vertexIndex++] = {v2->x, h, v2->z, (v->x + v2->x) / 2, 0, (v->z + v2->z) / 2, uStage2[2], 1};
	vertices[vertexIndex++] = {v2->x, 0, v2->z, (v->x + v2->x) / 2, 0, (v->z + v2->z) / 2, uStage2[2], 1};
	vertices[vertexIndex++] = {v->x, 0, v->z, (v->x + v2->x) / 2, 0, (v->z + v2->z) / 2, uStage[0], 0};
	drawableSize[2] = vertexIndex - drawableOffset[2];

	vertexCount = vertexIndex;
}

RoundedRegTriPrism::~RoundedRegTriPrism()
{
	 delete[] vertices;
}

Vertex *RoundedRegTriPrism::getVertices()
{
	return vertices;
}

int RoundedRegTriPrism::getVertexCount()
{
	return vertexCount;
}

int RoundedRegTriPrism::getDrawableCount()
{
	return 3;
}

int RoundedRegTriPrism::getDrawableOffset(int index)
{
	return drawableOffset[index];
}

int RoundedRegTriPrism::getDrawableSize(int index)
{
	return drawableSize[index];
}
