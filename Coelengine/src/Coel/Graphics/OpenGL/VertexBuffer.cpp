#include "clpch.hpp"
#include "VertexBuffer.hpp"

#include <glad/glad.h>

namespace Coel
{
namespace Graphics
{
namespace OpenGL
{
unsigned int VertexBuffer::s_bufferCount = 0, VertexBuffer::s_vao = 0;

VertexBuffer::VertexBuffer()
	: m_id(0), m_dim(0), m_count(0)
{
}

VertexBuffer::VertexBuffer(const float *data, unsigned int count, unsigned int dim)
	: m_id(0), m_dim(0), m_count(0)
{
	create(data, count, dim);
}

void VertexBuffer::create(const float *data, unsigned int count, unsigned int dim)
{
	m_dim = dim;
	m_count = count;

	if (!s_vao)
		glGenVertexArrays(1, &s_vao);

	glBindVertexArray(s_vao);

	if (!m_id)
	{
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * count * dim, data, GL_STATIC_DRAW);
		glEnableVertexAttribArray(s_bufferCount);
		glVertexAttribPointer(s_bufferCount, dim, GL_FLOAT, GL_FALSE, 0, (void *)0);
		s_bufferCount++;
	}
}

void VertexBuffer::bind()
{
	glBindVertexArray(s_vao);
}

void VertexBuffer::unbind()
{
	glBindVertexArray(0);
}
} // namespace OpenGL
} // namespace Graphics
} // namespace Coel
