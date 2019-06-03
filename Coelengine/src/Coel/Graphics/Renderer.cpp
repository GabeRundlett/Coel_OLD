#include "pch.hpp"

#include "Buffers.hpp"
#include "Renderable.hpp"
#include "Coel/Internals/Utilities/Log.hpp"
#include <glad/glad.h>
#include "Renderer.hpp"

#define GLCall(x)                           \
	glGetError();                           \
	x;                                      \
	if (!GLLogCall(#x, __FILE__, __LINE__)) \
		while (1) {}

bool GLLogCall(const char *function, const char *file, int line)
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		LOG_ERROR(Renderer, " OpenGL error\n");
		printf("(%d) line: %d", (int)error, line);
		return false;
	}
	return true;
}

#define RENDERER_VERTEX_SIZE sizeof(Coel::Graphics::VertexData)
#define RENDERER_MAX_SPRITES 60000
#define RENDERER_SPRITE_SIZE (RENDERER_VERTEX_SIZE * 4)
#define RENDERER_BUFFER_SIZE (RENDERER_SPRITE_SIZE * RENDERER_MAX_SPRITES)
#define RENDERER_INDICES_SIZE (RENDERER_MAX_SPRITES * 6)

namespace Coel {
	namespace Graphics {
		namespace Renderer {
			static IndexBuffer s_ibo;
			static VertexData *s_bufferPointer = nullptr;
			static unsigned int s_vaoID, s_vboID, s_indexCount;
			static unsigned int s_indices[RENDERER_INDICES_SIZE];
			int init()
			{
				LOG_INFO(Renderer, "Initializing renderer...\n");
				GLCall(glGenVertexArrays(1, &s_vaoID));
				glBindVertexArray(s_vaoID);

				GLCall(glGenBuffers(1, &s_vboID));
				glBindBuffer(GL_ARRAY_BUFFER, s_vboID);
				glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);

				glEnableVertexAttribArray(0);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const void *)0);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const void *)(3 * sizeof(float)));

				int offset = 0;
				for (int i = 0; i < RENDERER_INDICES_SIZE; i += 6) {
					s_indices[i] = offset + 0;
					s_indices[i + 1] = offset + 1;
					s_indices[i + 2] = offset + 2;
					s_indices[i + 3] = offset + 1;
					s_indices[i + 4] = offset + 3;
					s_indices[i + 5] = offset + 2;

					offset += 4;
				}

				s_ibo = createIndexBuffer(s_indices, RENDERER_INDICES_SIZE);

				LOG_SUCCESS(Renderer, "Initialized renderer\n");
				return 1;
			}
			void begin()
			{
				glClear(GL_COLOR_BUFFER_BIT);
				glBindBuffer(GL_ARRAY_BUFFER, s_vboID);
				s_bufferPointer = (VertexData *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
			}
			void end()
			{
				glUnmapBuffer(GL_ARRAY_BUFFER);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
			void flush()
			{
				glBindVertexArray(s_vaoID);
				Graphics::bindBuffer(&s_ibo);
				glDrawElements(GL_TRIANGLES, s_indexCount, GL_UNSIGNED_INT, nullptr);
				s_indexCount = 0;
			}
			void drawRect(float x, float y, float w, float h, unsigned int color)
			{
				if (s_indexCount > RENDERER_INDICES_SIZE - 1) {
					end();
					flush();
					begin();
				}
				s_bufferPointer->pos = {x, y, 0.f};
				s_bufferPointer->tex = {0.f, 0.f};
				s_bufferPointer++;

				s_bufferPointer->pos = {x + w, y, 0.f};
				s_bufferPointer->tex = {1.f, 0.f};
				s_bufferPointer++;

				s_bufferPointer->pos = {x, y + h, 0.f};
				s_bufferPointer->tex = {0.f, 1.f};
				s_bufferPointer++;

				s_bufferPointer->pos = {x + w, y + h, 0.f};
				s_bufferPointer->tex = {1.f, 1.f};
				s_bufferPointer++;

				s_indexCount += 6;
			}
			void resizeViewport(float x, float y, float w, float h)
			{
				glViewport(x, y, w, h);
			}
		} // namespace Renderer
	} // namespace Graphics
} // namespace Coel
