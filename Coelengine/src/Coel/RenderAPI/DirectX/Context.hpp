#pragma once
#include "Coel/RenderAPI/Context.hpp"

struct GLFWwindow;

namespace Coel
{
namespace RenderAPI
{
namespace DirectX
{
class Context : public RenderAPI::Context
{
	GLFWwindow *m_window;

public:
	Context(GLFWwindow *window);

	void init() override;
	void swap() override;
};
} // namespace DirectX
} // namespace RenderAPI
} // namespace Coel
