#pragma once

#include <QMatrix4x4>
#include <core.hpp>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <part/Part.hpp>
#include <QVector>
#include <QOpenGLTexture>
#include <QImage>

class PartDrawable: protected QOpenGLFunctions
{
	QOpenGLBuffer* vertexBuffer;
	QMatrix4x4 modelMatrix;
	Part* part;

	bool visible;

	Vertex* oldVertices;

public:
	PartDrawable(Part* part);
	~PartDrawable();

	void unbindPart();
	void updatePart();

	QMatrix4x4 getModelMatrix();

	void draw(QOpenGLShaderProgram& program, QMatrix4x4& transform);

	bool isVisible() const
	{
		return visible;
	}

	void setVisible(bool value)
	{
		visible = value;
	}

	QString getName() const
	{
		return part->getName().c_str();
	}

	Part* getPart() const
	{
		return part;
	}

	void releaseOldBuffer();
};