#include "pch.hpp"

#include "Buffers.hpp"
#include "Renderable.hpp"
#include "Coel/Internals/Utilities/Log.hpp"
#include <glad/glad.h>
#include "Renderer.hpp"

namespace Coel {
	namespace Graphics {
		namespace Renderer {
			constexpr static const unsigned int s_maxSprites = 60000,
												s_vertexSize = sizeof(VertexData),
												s_spriteSize = s_vertexSize * 4,
												s_indicesSize = s_maxSprites * 6,
												s_bufferSize = s_spriteSize * s_maxSprites;
			static IndexBuffer s_ibo;
			static VertexData *s_bufferPointer = nullptr;
			static unsigned int s_vaoID, s_vboID, s_indexCount;
			static unsigned short s_indices[s_indicesSize];
			int init()
			{
				LOG_INFO(Renderer, "Initializing renderer...\n");
				glGenVertexArrays(1, &s_vaoID);
				glBindVertexArray(s_vaoID);

				glGenBuffers(1, &s_vboID);
				glBindBuffer(GL_ARRAY_BUFFER, s_vboID);
				glBufferData(GL_ARRAY_BUFFER, s_bufferSize, NULL, GL_DYNAMIC_DRAW);

				glEnableVertexAttribArray(0);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, s_vertexSize, (const void *)0);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, s_vertexSize, (const void *)(3 * sizeof(float)));

				int offset = 0;
				for (int i = 0; i < s_indicesSize; i += 6) {
					s_indices[i] = offset + 0;
					s_indices[i + 1] = offset + 1;
					s_indices[i + 2] = offset + 2;
					s_indices[i + 3] = offset + 1;
					s_indices[i + 4] = offset + 3;
					s_indices[i + 5] = offset + 2;

					offset += 4;
				}

				s_ibo = createIndexBuffer(s_indices, s_indicesSize);

				LOG_SUCCESS(Renderer, "Initialized renderer\n");
				return 1;
			}
			void clear()
			{
				glClear(GL_COLOR_BUFFER_BIT);
			}
			void begin()
			{
				glBindBuffer(GL_ARRAY_BUFFER, s_vboID);
				s_bufferPointer = (VertexData *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
			}
			void end()
			{
				glUnmapBuffer(GL_ARRAY_BUFFER);
			}
			void flush()
			{
				glBindVertexArray(s_vaoID);
				Graphics::bindBuffer(&s_ibo);
				glDrawElements(GL_TRIANGLES, s_indexCount, GL_UNSIGNED_SHORT, nullptr);
				s_indexCount = 0;
			}
			void drawRect(float x, float y, float w, float h, unsigned int color)
			{
				if (s_indexCount > s_indicesSize - 1) {
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
