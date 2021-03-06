#include "gepch.h"

#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGLShader.h"

namespace GooE {

	static GLenum ShaderTypeFromString(const std::string& type) {
		GOOE_PROFILE_FUNCTION();

		if (type == "vertex") return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

		GOOE_CORE_ERROR("Unknown shader type: {0}", type);
		GOOE_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& path) {
		GOOE_PROFILE_FUNCTION();

		std::string source = ReadFile(path);
		auto shaders = PreProcess(source);
		Compile(shaders);

		// extract name from `path`
		auto lastSlash = path.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;

		auto lastDot = path.rfind('.');
		auto count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;

		name = path.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) : name(name) {
		GOOE_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;

		Compile(sources);
	}

	OpenGLShader::~OpenGLShader() {
		GOOE_PROFILE_FUNCTION();

		glDeleteProgram(rendererId);
	}

	std::string OpenGLShader::ReadFile(const std::string& file) {
		GOOE_PROFILE_FUNCTION();

		std::string result;
		std::ifstream in(file, std::ios::in | std::ios::binary);
		if (in) {
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1) {
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], result.size());
				in.close();
			} else {
				GOOE_CORE_ERROR("Could not read from file: {0}", file);
			}
		} else {
			GOOE_CORE_ERROR("Could not open file: {0}", file);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source) {
		GOOE_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		while (pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos);
			GOOE_CORE_ASSERT(eol != std::string::npos, "Syntax error");

			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			GOOE_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			GOOE_CORE_ASSERT(nextLinePos != std::string::npos, "Shader syntax error!");
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(
				nextLinePos,
				pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos)
			);
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources) {
		GOOE_PROFILE_FUNCTION();

		GOOE_CORE_ASSERT(shaderSources.size() <= 2, "Only 2 shaders at maximum are supported for now!");
		GLuint program = glCreateProgram();
		std::array<GLenum, 2> glShaderIds;
		int glShaderIdIndex = 0;

		for (auto& kv : shaderSources) {
			GLenum shaderType = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(shaderType);

			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				// Use the infoLog as you see fit.

				// In this simple program, we'll just leave
				GOOE_CORE_ERROR("{0}", infoLog.data());
				GOOE_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(program, shader);
			glShaderIds[glShaderIdIndex++] = shader;
		}

		rendererId = program;

		// Link our program
		glLinkProgram(rendererId);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(rendererId, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(rendererId, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(rendererId, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(rendererId);
			// Don't leak shaders either.
			for (auto id : glShaderIds)
				glDeleteShader(id);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave

			GOOE_CORE_ERROR("{0}", infoLog.data());
			GOOE_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		// Always detach shaders after a successful link.
		for (auto id : glShaderIds) {
			glDetachShader(rendererId, id);
			glDeleteShader(id);
		}
	}

	void OpenGLShader::Bind() const {
		GOOE_PROFILE_FUNCTION();

		glUseProgram(rendererId);
	}

	void OpenGLShader::Unbind() const {
		GOOE_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	void OpenGLShader::SetFloat(const std::string& name, const float value) {
		GOOE_PROFILE_FUNCTION();

		glUniform1f(GetUniformLocation(name.c_str()), value);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value) {
		GOOE_PROFILE_FUNCTION();

		glUniform4f(GetUniformLocation(name.c_str()), value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value) {
		GOOE_PROFILE_FUNCTION();

		glUniform3f(GetUniformLocation(name.c_str()), value.x, value.y, value.z);
	}

	void OpenGLShader::SetIntArray(const std::string& name, const int* values, const uint32_t count) {
		GOOE_PROFILE_FUNCTION();

		glUniform1iv(GetUniformLocation(name.c_str()), count, values);
	}

	void OpenGLShader::SetInt(const std::string& name, const int value) {
		GOOE_PROFILE_FUNCTION();

		glUniform1i(GetUniformLocation(name.c_str()), value);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value) {
		GOOE_PROFILE_FUNCTION();

		glUniformMatrix4fv(GetUniformLocation(name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformInt(const std::string name, const int value) {
		glUniform1i(GetUniformLocation(name.c_str()), value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string name, const float value) {
		glUniform1f(GetUniformLocation(name.c_str()), value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string name, const glm::vec3& value) {
		glUniform2f(GetUniformLocation(name.c_str()), value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string name, const glm::vec3& value) {
		glUniform3f(GetUniformLocation(name.c_str()), value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string name, const glm::vec4& value) {
		glUniform4f(GetUniformLocation(name.c_str()), value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string name, const glm::mat3& value) {
		glUniformMatrix3fv(GetUniformLocation(name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformMat4(const std::string name, const glm::mat4& value) {
		glUniformMatrix4fv(GetUniformLocation(name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	const int OpenGLShader::GetUniformLocation(const std::string name) {
		GOOE_PROFILE_FUNCTION();

		if (uniformLocationsCache.find(name) != uniformLocationsCache.end())
			return uniformLocationsCache[name];

		GLint location = glGetUniformLocation(rendererId, name.c_str());
		uniformLocationsCache[name] = location;

		if (location == -1) GOOE_CORE_WARN("Uniform {0} doesn't exist!", name.c_str());

		return location;
	}
}