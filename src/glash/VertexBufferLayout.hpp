#pragma once

#include "glash/glash_pch.hpp"
#include "glash/Enums.hpp"

namespace glash
{
	struct VertexBufferElement
	{
		uint32_t count;
		GLAttributeType type;
		bool normalized;
		uint32_t size;
	};

	class VertexBufferLayout
	{
	public:

		VertexBufferLayout()
			: m_Stride(0) {}


		template <class T>
		inline void Push(uint32_t count)
		{
			static_assert(false, "type not supported");
		}

		template <>
		inline void Push <float> (uint32_t count)
		{
			uint32_t size = sizeOfType(GLAttributeType::FLOAT);
			m_Elements.push_back({ .count = count, .type = GLAttributeType::FLOAT, .normalized = false, .size = size});
			m_Stride += count * size;
		}

		template <>
		inline void Push<uint32_t>(uint32_t count)
		{
			uint32_t size = sizeOfType(GLAttributeType::UNSIGNED_INT);
			m_Elements.push_back({ .count = count, .type = GLAttributeType::UNSIGNED_INT, .normalized = false, .size = size});
			m_Stride += count * size;
		}

		template <>
		inline void Push<unsigned char>(uint32_t count)
		{
			uint32_t size = sizeOfType(GLAttributeType::UNSIGNED_BYTE);
			m_Elements.push_back({ .count = count, .type = GLAttributeType::UNSIGNED_BYTE, .normalized = true, .size = size });
			m_Stride += count * size;

		}

		inline uint32_t GetStride() const 
		{
			return m_Stride;
		}

		inline const std::vector<VertexBufferElement>& GetElements() const
		{
			return m_Elements;
		};

	private:
		inline uint32_t sizeOfType(GLAttributeType type)
		{
			switch (type)
			{
			case UNSIGNED_BYTE: case BYTE: return sizeof(char);
			case UNSIGNED_INT: case INT: return sizeof(int32_t);
			case UNSIGNED_SHORT: case SHORT: return sizeof(int16_t);
			case FLOAT: return sizeof(float);
			case DOUBLE: return sizeof(double);
			default: return 0;
			}
		}


	private:
		uint32_t m_Stride;
		std::vector<VertexBufferElement> m_Elements;
	};
}