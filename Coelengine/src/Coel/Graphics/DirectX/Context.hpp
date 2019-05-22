#pragma once
#include "Coel/Graphics/Context.hpp"

struct GLFWwindow;

namespace Coel
{
namespace Graphics
{
namespace DirectX
{
class Context : public Graphics::Context
{
	GLFWwindow *m_window;

public:
	Context(GLFWwindow *window);

	void init() override;
	void swap() override;
};
} // namespace DirectX
} // namespace Graphics
} // namespace Coel
