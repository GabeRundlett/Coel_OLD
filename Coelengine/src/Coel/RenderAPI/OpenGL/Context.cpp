#include "clpch.hpp"
#include "Context.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Coel
{
namespace RenderAPI
{
namespace OpenGL
{
Context::Context(GLFWwindow *window)
	: m_window(window)
{
}

void Context::init()
{
	glfwMakeContextCurrent(m_window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Context::swap()
{
	glfwSwapBuffers(m_window);
}
} // namespace OpenGL
} // namespace RenderAPI
} // namespace Coel
