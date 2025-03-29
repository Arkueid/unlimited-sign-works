#include <render/PartDrawable.hpp>
#include <stddef.h>
#include <utils/TextureManager.hpp>
#include <utils/VertexBufferManager.hpp>

PartDrawable::PartDrawable(Part* part): part(part), vertexBuffer(nullptr), visible(true)
{
	initializeOpenGLFunctions();

	vertexBuffer = VertexBufferManager::registerVertexBuffer(part->getVertices(), part->getVertexCount());

	modelMatrix.setToIdentity();
	modelMatrix.translate(part->getOriginX(), part->getOriginY(), part->getOriginZ());
	modelMatrix.rotate(part->getRotationX(), 1, 0, 0);
	modelMatrix.rotate(part->getRotationY(), 0, 1, 0);
	modelMatrix.rotate(part->getRotationZ(), 0, 0, 1);

	for (int i = 0; i < part->getDrawableCount(); i++)
	{
		const std::string& path = part->getTexturePath(i);
		textures.append(TextureManager::loadTexture(path.c_str()));
	}
}

PartDrawable::~PartDrawable()
{
	VertexBufferManager::unregisterVertexBuffer(part->getVertices());
}

void PartDrawable::draw(QOpenGLShaderProgram& program, QMatrix4x4& transform)
{
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
	for (int i = 0; i < part->getDrawableCount(); i++)
	{
		glActiveTexture(GL_TEXTURE0);
		textures[i]->bind();
		glDrawArrays(GL_TRIANGLES, part->getDrawableOffset(i), part->getDrawableSize(i));
		textures[i]->release();
	}
	vertexBuffer->release();
}

