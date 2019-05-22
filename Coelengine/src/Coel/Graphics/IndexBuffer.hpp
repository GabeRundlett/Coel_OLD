#pragma once

namespace Coel
{
namespace Graphics
{
class IndexBuffer
{
public:
	virtual void create(const unsigned int *data, unsigned int count) = 0;
	virtual void bind() = 0;
	virtual void unbind() = 0;
};
} // namespace Graphics
} // namespace Coel
