#pragma once

#include "glash/glash_pch.hpp"
#include "glash/logger.hpp"

#include <optional>

namespace glash
{
    namespace fs = std::filesystem;
    inline fs::path ROOT_PATH = fs::path("./");

    inline std::unique_ptr<char[]> ReadFile(const fs::path& relative_path)
    {
        fs::path full_path = ROOT_PATH / relative_path;

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
} // namespace glash
