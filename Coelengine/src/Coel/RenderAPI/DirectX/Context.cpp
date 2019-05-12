#include "clpch.hpp"
#include "Context.hpp"

#include <GLFW/glfw3.h>

namespace Coel
{
namespace RenderAPI
{
namespace DirectX
{
Context::Context(GLFWwindow *window)
	: m_window(window)
{
}

void Context::init()
{
}

void Context::swap()
{
	glfwSwapBuffers(m_window);
}
} // namespace DirectX
} // namespace RenderAPI
} // namespace Coel
