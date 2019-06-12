#include "pch.hpp"

#include "Coel/Internals/Utilities/Log.hpp"
#include <glad/glad.h>
#include "Shader.hpp"

namespace Coel {
	namespace Graphics {
		namespace Shader {
			Data create(const char *vertSource, const char *fragSource)
			{
				unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
				glShaderSource(vertexShader, 1, &vertSource, 0);
				glCompileShader(vertexShader);
#ifdef CONFIG_DEBUG
				int temp = 0;
				glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &temp);
				if (temp == GL_FALSE) {
					temp = 0;
					glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &temp);
					std::vector<char> infoLog(temp);
					glGetShaderInfoLog(vertexShader, temp, &temp, &infoLog[0]);
					glDeleteShader(vertexShader);
					LOG_ERROR(Shader, "Vertex Shader failed to compile\n");
					std::cout << infoLog.data() << '\n';
					return {0};
				}
#endif
				unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
				glShaderSource(fragmentShader, 1, &fragSource, 0);
				glCompileShader(fragmentShader);
#ifdef CONFIG_DEBUG
				glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &temp);
				if (temp == GL_FALSE) {
					glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &temp);
					std::vector<char> infoLog(temp);
					glGetShaderInfoLog(fragmentShader, temp, &temp, &infoLog[0]);
					glDeleteShader(fragmentShader);
					glDeleteShader(vertexShader);
					LOG_ERROR(Shader, "Fragment Shader failed to compile\n");
					std::cout << infoLog.data() << '\n';
					return {0};
				}
#endif
				Data result;
				result.id = glCreateProgram();

				glAttachShader(result.id, vertexShader);
				glAttachShader(result.id, fragmentShader);
				glLinkProgram(result.id);
#ifdef CONFIG_DEBUG
				glGetProgramiv(result.id, GL_LINK_STATUS, (int *)&temp);
				if (temp == GL_FALSE) {
					glGetProgramiv(result.id, GL_INFO_LOG_LENGTH, &temp);
					std::vector<char> infoLog(temp);
					glGetProgramInfoLog(result.id, temp, &temp, &infoLog[0]);
					glDeleteProgram(result.id);
					glDeleteShader(vertexShader);
					glDeleteShader(fragmentShader);
					LOG_ERROR(Shader, "Failed to create Shader Program\n");
					std::cout << infoLog.data() << '\n';
					return {0};
				}
#endif
				glDetachShader(result.id, vertexShader);
				glDetachShader(result.id, fragmentShader);
				return result;
			}
			void bind(Data *d)
			{
				glUseProgram(d->id);
			}
			void unbind(Data *d)
			{
				glUseProgram(0);
			}
			void destroy(Data *d)
			{
				glDeleteProgram(d->id);
			}
			void sendMat4(Data *d, const char *name, const float *data)
			{
				glUniformMatrix4fv(glGetUniformLocation(d->id, name), 1, GL_FALSE, data);
			}
		} // namespace Shader
	} // namespace Graphics
} // namespace Coel
