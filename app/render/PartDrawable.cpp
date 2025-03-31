#include <render/PartDrawable.hpp>
#include <stddef.h>
#include <utils/TextureManager.hpp>
#include <utils/VertexBufferManager.hpp>

PartDrawable::PartDrawable(Part* part): part(part), vertexBuffer(nullptr), visible(true)
{
	initializeOpenGLFunctions();

	vertexBuffer = VertexBufferManager::registerVertexBuffer(part->getVertices(), part->getVertexCount());

	QStringList* texturePaths = TextureManager::getTexturePaths();
	for (int i : part->getTextureRefs())
	{
		TextureManager::loadTexture((*texturePaths)[i]);
	}
}

PartDrawable::~PartDrawable()
{
	VertexBufferManager::unregisterVertexBuffer(part->getVertices());
}

void PartDrawable::draw(QOpenGLShaderProgram& program, QMatrix4x4& transform)
{
	modelMatrix.setToIdentity();
	modelMatrix.translate(part->getOriginX(), part->getOriginY(), part->getOriginZ());
	modelMatrix.rotate(part->getRotationX(), 1, 0, 0);
	modelMatrix.rotate(part->getRotationY(), 0, 1, 0);
	modelMatrix.rotate(part->getRotationZ(), 0, 0, 1);
	QMatrix4x4 scale;
	scale.setToIdentity();
	scale.scale(0.0001f);
	scale = transform * scale * modelMatrix;
	program.setUniformValue("model", scale);
	program.setUniformValue("normalMatrix", scale.normalMatrix());
	vertexBuffer->bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, nx));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, u));
	const std::vector<int> textureRefs = part->getTextureRefs();
	QOpenGLTexture* texture;
	for (int i = 0; i < part->getDrawableCount(); i++)
	{
		glActiveTexture(GL_TEXTURE0);
		texture = TextureManager::getTexture(textureRefs[i]);
		texture->bind();
		glDrawArrays(GL_TRIANGLES, part->getDrawableOffset(i), part->getDrawableSize(i));
		texture->release();
	}
	vertexBuffer->release();
}

