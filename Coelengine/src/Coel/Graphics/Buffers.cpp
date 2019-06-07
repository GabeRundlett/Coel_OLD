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

		VertexBuffer createStaticVertexBuffer(float *data, unsigned int size)
		{
			LOG_INFO(VertexBuffer, "Creating static vertex buffer...\n");
			VertexBuffer result;
			result.size = size;
			glCreateBuffers(1, &result.id);
			glBindBuffer(GL_ARRAY_BUFFER, result.id);
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			LOG_SUCCESS(VertexBuffer, "Created static vertex buffer\n");
			return result;
		}
		VertexBuffer createDynamicVertexBuffer(unsigned int size)
		{
			LOG_INFO(VertexBuffer, "Creating dynamic vertex buffer...\n");
			VertexBuffer result;
			glCreateBuffers(1, &result.id);
			glBindBuffer(GL_ARRAY_BUFFER, result.id);
			resizeVertexBuffer(&result, size);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			LOG_SUCCESS(VertexBuffer, "Created dynamic vertex buffer\n");
			return result;
		}
		void bindBuffer(VertexBuffer *b)
		{
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
		void resizeVertexBuffer(VertexBuffer *b, unsigned int size)
		{
			b->size = size;
			glBindBuffer(GL_ARRAY_BUFFER, b->id);
			glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
		}

		//----------------------------------------------------------------//
		//                          IndexBuffer                           //
		//----------------------------------------------------------------//

		IndexBuffer createIndexBuffer(unsigned short *data, unsigned int count)
		{
			LOG_INFO(IndexBuffer, "Creating index buffer...\n");
			IndexBuffer result;
			result.count = count;
			glCreateBuffers(1, &result.id);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result.id);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned short), data, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
