#pragma once
#include "Coel/Graphics/IndexBuffer.hpp"

namespace Coel
{
namespace Graphics
{
namespace OpenGL
{
class IndexBuffer : public Graphics::IndexBuffer
{
	unsigned int m_id, m_count;

public:
	IndexBuffer();
	IndexBuffer(const unsigned int *data, unsigned int count);
	void create(const unsigned int *data, unsigned int count);
	void bind();
	void unbind();
};
} // namespace OpenGL
} // namespace Graphics
} // namespace Coel
