#pragma once
#include "Coel/Graphics/VertexBuffer.hpp"

namespace Coel
{
namespace Graphics
{
namespace OpenGL
{
class VertexBuffer : public Graphics::VertexBuffer
{
	static unsigned int s_bufferCount, s_vao;
	unsigned int m_id, m_dim, m_count;

public:
	VertexBuffer();
	VertexBuffer(const float *data, unsigned int count, unsigned int dim);
	void create(const float *data, unsigned int count, unsigned int dim);
	void bind();
	void unbind();
};
} // namespace OpenGL
} // namespace Graphics
} // namespace Coel
