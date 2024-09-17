#pragma once

#include "glash/glash_pch.hpp"
#include "glash/Enums.hpp"

namespace glash
{
	class Texture
	{
	public:
		Texture();
		Texture(const std::string& path);
		~Texture();

		void Bind(const GLuint slot = 0) const;
		void Unbind() const;
	
		struct StbiImageDeleter {
			void operator()(unsigned char* image);
	};

	private:
		GLuint m_RendererID;
		std::string m_FilePath;
		std::unique_ptr<unsigned char[], StbiImageDeleter> m_LocalBuffer;
		int m_Width, m_Height, m_Bits;
	};
}