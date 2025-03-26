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

class Drawable: protected QOpenGLFunctions
{
	QOpenGLBuffer vertexBuffer;
	QMatrix4x4 modelMatrix;
	QVector<QOpenGLTexture*> textures;
	Part* part;

public:
	Drawable(Part* part);
	~Drawable();

	void draw(QOpenGLShaderProgram& program);
};