#pragma once

namespace Coel
{
namespace Graphics
{
enum class RenderAPI
{
	OpenGL,
	DirectX,
	Vulkan,
	Metal
};

class Context
{
public:
	virtual void init() = 0;
	virtual void swap() = 0;
	virtual RenderAPI getAPI() = 0;
};
} // namespace Graphics
} // namespace Coel
