#pragma once

#include "glash/glash_pch.hpp"

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
            throw std::runtime_error("Couldn't open file: " + full_path.string());
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        auto buffer = std::make_unique<char[]>(size + 1);
        if (!file.read(buffer.get(), size))
        {
            throw std::runtime_error("Error reading file: " + full_path.string());
        }

        buffer[size] = '\0';
        return buffer;
    }
} // namespace glash
