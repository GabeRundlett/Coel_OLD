#include "clpch.hpp"
#include "Context.hpp"

#include <GLFW/glfw3.h>

namespace Coel
{
namespace Graphics
{
namespace Vulkan
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
} // namespace Vulkan
} // namespace Graphics
} // namespace Coel
