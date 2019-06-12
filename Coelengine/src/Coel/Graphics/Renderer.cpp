#include "pch.hpp"

#include "Buffers.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Renderable.hpp"
#include "Coel/Internals/Utilities/Log.hpp"
#include <glad/glad.h>

namespace Coel {
	namespace Graphics {
		namespace Renderer {
			constexpr static const unsigned int s_maxSprites = 16384,
												s_vertexSize = sizeof(Vertex::Data),
												s_spriteSize = s_vertexSize * 4,
												s_maxIndexCount = s_maxSprites * 6,
												s_bufferSize = s_spriteSize * s_maxSprites;
			static Shader::Data s_shader;
			static IndexBuffer::Data s_ibo;
			static VertexBuffer::Data s_vbo;
			static unsigned int s_vaoID, s_indexCount;
			static unsigned short s_indices[s_maxIndexCount];
			static Vertex::Data *s_bufferPointer = nullptr;
			static mat4 projectionMatrix;
			int init()
			{
				LOG_INFO(Renderer, "Initializing renderer...\n");
				glGenVertexArrays(1, &s_vaoID);
				glBindVertexArray(s_vaoID);

				s_vbo = VertexBuffer::create(s_bufferSize);
				glEnableVertexAttribArray(0);
				glEnableVertexAttribArray(1);
				glEnableVertexAttribArray(2);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, s_vertexSize, (const void *)0);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, s_vertexSize, (const void *)(3 * sizeof(float)));
				glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, s_vertexSize, (const void *)(5 * sizeof(float)));
				VertexBuffer::unbind(&s_vbo);

				projectionMatrix = mat4::ortho(0, 800, 600, 0, -1, 1);

				int offset = 0;
				for (int i = 0; i < s_maxIndexCount; i += 6) {
					s_indices[i] = offset + 0;
					s_indices[i + 1] = offset + 1;
					s_indices[i + 2] = offset + 2;
					s_indices[i + 3] = offset + 1;
					s_indices[i + 4] = offset + 3;
					s_indices[i + 5] = offset + 2;

					offset += 4;
				}

				s_ibo = IndexBuffer::create(s_indices, s_maxIndexCount);

				const char *vertexShaderSource
					= "#version 330 core\n"
					  "layout(location = 0) in vec3 pos;\n"
					  "layout(location = 1) in vec2 tex;\n"
					  "layout(location = 2) in vec4 col;\n"
					  "uniform mat4 projMatrix;"
					  "out vec2 v_tex;"
					  "out vec4 v_col;"
					  "void main()\n"
					  "{\n"
					  "    v_tex = tex;"
					  "    v_col = col;"
					  "    gl_Position = projMatrix * vec4(pos, 1.0);\n"
					  "}";

				const char *fragmentShaderSource
					= "#version 330 core\n"
					  "in vec2 v_tex;"
					  "in vec4 v_col;"
					  "out vec4 color;\n"
					  "void main()\n"
					  "{\n"
					  "    color = v_col;\n"
					  "}";

				s_shader = Shader::create(vertexShaderSource, fragmentShaderSource);

				LOG_SUCCESS(Renderer, "Initialized renderer\n");
				return 1;
			}
			void clear()
			{
				glClear(GL_COLOR_BUFFER_BIT);
			}
			void begin()
			{
				Shader::bind(&s_shader);
				Shader::sendMat4(&s_shader, "projMatrix", projectionMatrix.indices);
				VertexBuffer::bind(&s_vbo);
				s_bufferPointer = (Vertex::Data *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
			}
			void end()
			{
				glUnmapBuffer(GL_ARRAY_BUFFER);
			}
			void flush()
			{
				glBindVertexArray(s_vaoID);
				glDrawElements(GL_TRIANGLES, s_indexCount, GL_UNSIGNED_SHORT, nullptr);
				s_indexCount = 0;
			}
			void drawRect(float x, float y, float w, float h, unsigned int color)
			{
				s_indexCount += 6;
				if (s_indexCount > s_maxIndexCount - 1) {
					end();
					flush();
					begin();
				}

				s_bufferPointer->pos = {x, y, 0.f};
				s_bufferPointer->tex = {0.f, 0.f};
				s_bufferPointer->col = color;
				s_bufferPointer++;

				s_bufferPointer->pos = {x + w, y, 0.f};
				s_bufferPointer->tex = {1.f, 0.f};
				s_bufferPointer->col = color;
				s_bufferPointer++;

				s_bufferPointer->pos = {x, y + h, 0.f};
				s_bufferPointer->tex = {0.f, 1.f};
				s_bufferPointer->col = color;
				s_bufferPointer++;

				s_bufferPointer->pos = {x + w, y + h, 0.f};
				s_bufferPointer->tex = {1.f, 1.f};
				s_bufferPointer->col = color;
				s_bufferPointer++;
			}
			void resizeViewport(int x, int y, int w, int h)
			{
				glViewport(x, y, w, h);
				projectionMatrix = mat4::ortho((float)x, (float)w, (float)h, (float)y, -1, 1);
			}
		} // namespace Renderer
	} // namespace Graphics
} // namespace Coel
