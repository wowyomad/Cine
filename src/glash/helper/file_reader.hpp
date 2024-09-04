#pragma once

#include "glash/glash_pch.hpp"
#include "glash/logger.hpp"


namespace glash
{
	namespace fs = std::filesystem;
	inline fs::path ROOT_PATH = fs::path("./");

	inline std::unique_ptr<char[]> ReadFile(const fs::path& filepath)
	{
		fs::path full_path = ROOT_PATH / filepath;

		std::ifstream file(full_path, std::ios::binary | std::ios::ate);
		if (!file)
		{
			const std::string message = "Couldn't open file: " + full_path.string();
			LOG_ERROR(fmt::runtime(message));
			return nullptr;
		}

		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		auto buffer = std::make_unique<char[]>(size + 1);
		if (!file.read(buffer.get(), size))
		{
			const std::string message = "Error reading file: " + full_path.string();
			LOG_ERROR(fmt::runtime(message));
			return nullptr;
		}

		buffer[size] = '\0';
		return buffer;
	}

	inline std::vector<ShaderSource> ParseGLShaders(const fs::path& filepath)
	{
		std::ifstream file(filepath);

		if (!file)
		{
			const std::string message = "Couldn't open file: " + filepath.string();
			LOG_ERROR(fmt::runtime(message));
			return {};
		}

		std::string line;
		std::stringstream vertexShaderStream, fragmentShaderStream;
		enum SHADER_TYPE type = SHADER_TYPE::NONE;

		while (getline(file, line))
		{
			if (line.empty()) continue;
			if (line.contains("#shader"))
			{
				if (line.contains("vertex"))
				{
					type = SHADER_TYPE::VERTEX_SHADER;
				}
				else if (line.contains("fragment"))
				{
					type = SHADER_TYPE::FRAGMENT_SHADER;
				}
			}
			else
			{
				if (type == SHADER_TYPE::VERTEX_SHADER)
				{
					vertexShaderStream << line << '\n';
				}
				else if (type == SHADER_TYPE::FRAGMENT_SHADER)
				{
					fragmentShaderStream << line << '\n';
				}
				else
				{
					LOG_ERROR("File {} doesn't begin with #shader <type>", filepath.string());
					return {};
				}
			}
		}

		std::vector<ShaderSource> sources
		{
			{
			SHADER_TYPE::VERTEX_SHADER,
			vertexShaderStream.str() 
			},
			{
			SHADER_TYPE::FRAGMENT_SHADER,
			fragmentShaderStream.str()
			}

		};
		return sources;
	}
} // namespace glash
