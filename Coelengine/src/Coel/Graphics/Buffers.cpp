#include "pch.hpp"

#include "Buffers.hpp"
#include <glad/glad.h>
#include "Coel/Internals/Utilities/Log.hpp"

namespace Coel {
	namespace Graphics {
		// All of this is OpenGL specific

		//----------------------------------------------------------------//
		//                          VertexBuffer                          //
		//----------------------------------------------------------------//
		namespace VertexBuffer {
			Data create(float *data, unsigned int size)
			{
				LOG_INFO(VertexBuffer, "Creating static vertex buffer...\n");
				Data result;
				result.size = size;
				glGenBuffers(1, &result.id);
				glBindBuffer(GL_ARRAY_BUFFER, result.id);
				glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
				LOG_SUCCESS(VertexBuffer, "Created static vertex buffer\n");
				return result;
			}
			Data create(unsigned int size)
			{
				LOG_INFO(VertexBuffer, "Creating dynamic vertex buffer...\n");
				Data result;
				glGenBuffers(1, &result.id);
				resize(&result, size);
				LOG_SUCCESS(VertexBuffer, "Created dynamic vertex buffer\n");
				return result;
			}
			void bind(Data *d)
			{
				glBindBuffer(GL_ARRAY_BUFFER, d->id);
			}
			void unbind(Data *d)
			{
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
			void destroy(Data *d)
			{
				glDeleteBuffers(1, &d->id);
			}
			void resize(Data *d, unsigned int size)
			{
				d->size = size;
				glBindBuffer(GL_ARRAY_BUFFER, d->id);
				glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
			}
		} // namespace VertexBuffer

		//----------------------------------------------------------------//
		//                          IndexBuffer                           //
		//----------------------------------------------------------------//
		namespace IndexBuffer {
			Data create(unsigned short *data, unsigned int count)
			{
				LOG_INFO(IndexBuffer, "Creating index buffer...\n");
				Data result;
				result.count = count;
				glGenBuffers(1, &result.id);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result.id);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned short), data, GL_STATIC_DRAW);
				LOG_SUCCESS(IndexBuffer, "Created index buffer\n");
				return result;
			}
			void bind(Data *d)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d->id);
			}
			void unbind(Data *d)
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}
			void destroy(Data *d)
			{
				glDeleteBuffers(1, &d->id);
			}
		} // namespace IndexBuffer
	} // namespace Graphics
} // namespace Coel
