#pragma once
#include "Coel/Graphics/Context.hpp"

struct GLFWwindow;

namespace Coel
{
namespace Graphics
{
namespace OpenGL
{
class Context : public Graphics::Context
{
	GLFWwindow *m_window;

public:
	Context(GLFWwindow *window);

	void init() override;
	void swap() override;
	RenderAPI getAPI() override;
};
} // namespace OpenGL
} // namespace Graphics
} // namespace Coel
