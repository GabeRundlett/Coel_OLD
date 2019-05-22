#include "clpch.hpp"
#include "IndexBuffer.hpp"
#include <glad/glad.h>

namespace Coel
{
namespace Graphics
{
namespace OpenGL
{
IndexBuffer::IndexBuffer()
	: m_id(0), m_count(0)
{
}

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count)
	: m_id(0), m_count(0)
{
	create(data, count);
}

void IndexBuffer::create(const unsigned int *data, unsigned int count)
{
	m_count = count;
	if (!m_id)
	{
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * count, data, GL_STATIC_DRAW);
	}
}

void IndexBuffer::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void IndexBuffer::unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
} // namespace OpenGL
} // namespace Graphics
} // namespace Coel
