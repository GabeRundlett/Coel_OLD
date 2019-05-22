#pragma once

namespace Coel
{
namespace Graphics
{
class Shader
{
	virtual void create(const char *vertPath, const char *fragPath) = 0;
	virtual void bind() = 0;
	virtual void unbind() = 0;
};
} // namespace Graphics
} // namespace Coel