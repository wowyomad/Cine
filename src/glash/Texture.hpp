#pragma once
#include "glash/Core/Core.hpp"

#include "glash/glash_pch.hpp"
#include "glash/Enums.hpp"

namespace glash
{
	class GLASH_API Texture_OLD
	{
	public:
		Texture_OLD();
		Texture_OLD(const std::string& path);
		~Texture_OLD();

		void LoadTexture(const std::string& path);

		void Bind(const GLuint slot = 0) const;
		void Unbind() const;

		int Width() const;
		int Height() const;
	
		struct StbiImageDeleter {
			void operator()(unsigned char* image);
	};

	private:
		GLuint m_RendererID;
		std::string m_Path;
		std::unique_ptr<unsigned char[], StbiImageDeleter> m_LocalBuffer;
		int m_Width, m_Height, m_Bits;
	};
}