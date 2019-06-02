#include "pch.hpp"

#include "Buffers.hpp"
#include <glad/glad.h>
#include "Coel/Internals/Utilities/Log.hpp"

namespace Coel {
	namespace Graphics {
		namespace Renderer {
			static Graphics::VertexBuffer vbo;
			static Graphics::IndexBuffer ibo;
			int init()
			{
				LOG_INFO(Renderer, "Initializing renderer...\n");
				float vertices[] = {
					-0.5f, -0.5f,
					0.5f, -0.5f,
					0.0f, 0.5f};
				vbo = Graphics::createVertexBuffer(vertices, sizeof(vertices));
				unsigned int indices[3] = {0, 1, 2};
				ibo = Graphics::createIndexBuffer(indices, 3);
				LOG_SUCCESS(Renderer, "Initialized renderer\n");
				return 1;
			}
			void update()
			{
				Graphics::bindBuffer(&vbo);
				glDrawElements(GL_TRIANGLES, ibo.count, GL_UNSIGNED_INT, nullptr);
			}
		} // namespace Renderer
	} // namespace Graphics
} // namespace Coel
