#pragma once

namespace Coel {
	namespace Graphics {
		namespace VertexBuffer {
			struct Data {
				unsigned int id;
				unsigned int size;
			};
			Data create(float *data, unsigned int size);
			Data create(unsigned int size);
			void bind(Data *d);
			void unbind(Data *d);
			void destroy(Data *d);
			void resize(Data *d, unsigned int size);
		} // namespace VertexBuffer
		namespace IndexBuffer {
			struct Data {
				unsigned int id;
				unsigned int count;
			};
			Data create(unsigned short *data, unsigned int size);
			void bind(Data *d);
			void unbind(Data *d);
			void destroy(Data *d);
		} // namespace IndexBuffer
	} // namespace Graphics
} // namespace Coel
