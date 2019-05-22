#pragma once

namespace Coel
{
namespace Graphics
{
class VertexBuffer
{
public:
	virtual void create(const float *data, unsigned int count, unsigned int dim) = 0;
	virtual void bind() = 0;
	virtual void unbind() = 0;
};
} // namespace Graphics
} // namespace Coel
