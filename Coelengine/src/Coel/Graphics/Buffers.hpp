#pragma once

namespace Coel {
	namespace Graphics {
		struct VertexBuffer {
			unsigned int id;
		};
		struct IndexBuffer {
			unsigned int id;
			unsigned int count;
		};

		VertexBuffer createVertexBuffer(float *data, unsigned int size);
		void bindBuffer(VertexBuffer *b);
		void unbindBuffer(VertexBuffer *b);
		void destroyBuffer(VertexBuffer *b);
		IndexBuffer createIndexBuffer(unsigned int *data, unsigned int size);
		void bindBuffer(IndexBuffer *b);
		void unbindBuffer(IndexBuffer *b);
		void destroyBuffer(IndexBuffer *b);
	} // namespace Graphics
} // namespace Coel
