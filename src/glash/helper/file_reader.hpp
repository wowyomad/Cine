#pragma once

#include "glash/glash_pch.hpp"
#include "glash/Log.hpp"
#include "glash/Enums.hpp"

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
			LOG_ERROR("Couldn't open file: {}", full_path.string());
			return nullptr;
		}

		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		auto buffer = std::make_unique<char[]>(size + 1);
		if (!file.read(buffer.get(), size))
		{
			LOG_ERROR("Error reading file: {}", full_path.string());
			return nullptr;
		}

		buffer[size] = '\0';
		return buffer;
	}
} // namespace glash
