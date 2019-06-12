#pragma once

namespace Coel {
	namespace Graphics {
		namespace Shader {
			struct Data {
				unsigned int id;
			};
			Data create(const char *vertSource, const char *fragSource);
			void bind(Data *d);
			void unbind(Data *d);
			void destroy(Data *d);
			void sendMat4(Data *d, const char *name, const float *data);
		} // namespace Shader
	} // namespace Graphics
} // namespace Coel
