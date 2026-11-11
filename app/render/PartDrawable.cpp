#include <render/PartDrawable.hpp>
#include <stddef.h>
#include <utils/TextureManager.hpp>
#include <utils/VertexBufferManager.hpp>
#include "PartDrawable.hpp"

PartDrawable::PartDrawable(Part* part): part(part), vertexBuffer(nullptr), visible(true), oldVertices(part->getVertices())
{
	initializeOpenGLFunctions();

	updatePart();
}

PartDrawable::~PartDrawable()
{
	if (vertexBuffer == nullptr)
	{
		return;
	}
	
	VertexBufferManager::unregisterVertexBuffer(part->getVertices());
}

void PartDrawable::unbindPart()
{
	if (vertexBuffer != nullptr)
	{
		VertexBufferManager::unregisterVertexBuffer(part->getVertices());
	}
	vertexBuffer = nullptr;
}

void PartDrawable::updatePart()
{
	vertexBuffer = VertexBufferManager::registerVertexBuffer(part->getVertices(), part->getVertexCount());
}
void PartDrawable::draw(QOpenGLShaderProgram& program, QMatrix4x4& transform)
{
	QMatrix4x4 model = transform * getModelMatrix(); 
	program.setUniformValue("model", model); // 设置部件位置
	program.setUniformValue("normalMatrix", model.normalMatrix()); // 设置表面法线变换
	vertexBuffer->bind(); // 绑定顶点缓冲对象
	glEnableVertexAttribArray(0); // 指定三维坐标信息
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1); // 指定法线信息
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, nx));
	glEnableVertexAttribArray(2); // 指定纹理坐标信息
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, u));
	const std::vector<int> textureRefs = part->getTextureRefs();
	QOpenGLTexture* texture;
	for (int i = 0; i < part->getDrawableCount(); i++) // 逐一绘制几何面
	{
		glActiveTexture(GL_TEXTURE0);
		texture = TextureManager::getTexture(textureRefs[i]); // 从纹理管理器中获取纹理
		texture->bind(); // 绑定纹理
		glDrawArrays(GL_TRIANGLES, part->getDrawableOffset(i), part->getDrawableSize(i));
		texture->release();
	}
	vertexBuffer->release();
}

void PartDrawable::releaseOldBuffer()
{
	VertexBufferManager::unregisterVertexBuffer(oldVertices);
	oldVertices = part->getVertices();
	vertexBuffer = nullptr;
}

QMatrix4x4 PartDrawable::getModelMatrix()
{
	modelMatrix.setToIdentity();
	modelMatrix.translate(part->getOriginX(), part->getOriginY(), part->getOriginZ());
	modelMatrix.rotate(part->getRotationX(), 1, 0, 0);
	modelMatrix.rotate(part->getRotationY(), 0, 1, 0);
	modelMatrix.rotate(part->getRotationZ(), 0, 0, 1);
	QMatrix4x4 scale;
	scale.setToIdentity();
	scale.scale(0.0001f);
	scale = scale * modelMatrix;
	return scale;
}
