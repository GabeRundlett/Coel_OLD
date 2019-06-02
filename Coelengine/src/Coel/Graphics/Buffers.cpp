#include "pch.hpp"

#include "Buffers.hpp"
#include <glad/glad.h>
#include "Coel/Internals/Utilities/Log.hpp"

namespace Coel {
	namespace Graphics {
		// All of this is OpenGL specific
		static unsigned int s_vertexArrayID = 0, s_vertexBufferCount = 0;

		VertexBuffer createVertexBuffer(float *data, unsigned int size)
		{
			LOG_INFO(VertexBuffer, "Creating vertex buffer...\n");
			if (!s_vertexArrayID) {
				glGenVertexArrays(1, &s_vertexArrayID);
				glBindVertexArray(s_vertexArrayID);
			}
			if (s_vertexBufferCount > 31) {
				LOG_ERROR(VertexBuffer, "Failed to create VBO, Too many vertex buffers\n");
				return {0};
			}
			VertexBuffer result;
			glCreateBuffers(1, &result.id);
			glBindBuffer(GL_ARRAY_BUFFER, result.id);
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

			glEnableVertexAttribArray(s_vertexBufferCount);
			glVertexAttribPointer(s_vertexBufferCount, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

			s_vertexBufferCount++;
			LOG_SUCCESS(VertexBuffer, "Created vertex buffer\n");
			return result;
		}
		void bindBuffer(VertexBuffer *b)
		{
			glBindVertexArray(s_vertexArrayID);
			glBindBuffer(GL_ARRAY_BUFFER, b->id);
		}
		void unbindBuffer(VertexBuffer *b)
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		void destroyBuffer(VertexBuffer *b)
		{
			glDeleteBuffers(1, &b->id);
		}
		IndexBuffer createIndexBuffer(unsigned int *data, unsigned int count)
		{
			LOG_INFO(IndexBuffer, "Creating index buffer...\n");
			IndexBuffer result;
			result.count = count;
			glCreateBuffers(1, &result.id);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result.id);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
			LOG_SUCCESS(IndexBuffer, "Created index buffer\n");
			return result;
		}
		void bindBuffer(IndexBuffer *b)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, b->id);
		}
		void unbindBuffer(IndexBuffer *b)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
		void destroyBuffer(IndexBuffer *b)
		{
			glDeleteBuffers(1, &b->id);
		}
	} // namespace Graphics
} // namespace Coel
