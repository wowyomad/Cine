#pragma once

#include "glash/Core/Core.hpp"

namespace Cine
{
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat4, Int, Int2, Int3, Int4, Bool
	};

	

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return	sizeof(float);
		case ShaderDataType::Float2:   return	sizeof(float) * 2;
		case ShaderDataType::Float3:   return	sizeof(float) * 3;
		case ShaderDataType::Float4:   return	sizeof(float) * 4;
		case ShaderDataType::Mat4:     return	sizeof(float) * 4 * 4;
		case ShaderDataType::Int:      return	sizeof(int);
		case ShaderDataType::Int2:     return	sizeof(int) * 2;
		case ShaderDataType::Int3:     return	sizeof(int) * 3;
		case ShaderDataType::Int4:     return	sizeof(int) * 4;
		case ShaderDataType::Bool:     return	sizeof(bool);
		}

		GLASH_CORE_ASSERT(false, "Uknnown type: {}", (uint32_t)type);
		return 0;
	}

	struct BufferElement
	{
		ShaderDataType Type;
		std::string Name;
		uint32_t Size;
		uint32_t Offset;
		bool Normalized;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Type(type), Name(name), Normalized(normalized), Offset(0), Size(ShaderDataTypeSize(type))
		{ }

		uint32_t GetCount() const
		{
			switch (Type)
			{
			case ShaderDataType::Float:    return	1;
			case ShaderDataType::Float2:   return	2;
			case ShaderDataType::Float3:   return	3;
			case ShaderDataType::Float4:   return	4;
			case ShaderDataType::Mat4:     return	4 * 4;
			case ShaderDataType::Int:      return	1;
			case ShaderDataType::Int2:     return	2;
			case ShaderDataType::Int3:     return	3;
			case ShaderDataType::Int4:     return	4;
			case ShaderDataType::Bool:     return	1;
			}

			GLASH_CORE_ASSERT(false, "Uknnown type: {}", (uint32_t)Type);
			return 0;
		}

	};

	class BufferLayout
	{
	public:
		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements)
		{
			CalculateOffsetAndStride(); 
		}

		inline void SetElements(const std::vector<BufferElement>& elements) { m_Elements = elements; }
		inline void SetElements(const std::initializer_list<BufferElement>& elements) { m_Elements = elements; }
		inline const std::vector<BufferElement>& Elements() const { return m_Elements; }
		inline const uint32_t Stride() const { return m_Stride;  }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		void CalculateOffsetAndStride()
		{
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = m_Stride;
				m_Stride += element.Size;
			}
		}

	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void SetData(const void* data, uint32_t size) = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(const float* vertices, uint32_t size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(const uint32_t* indices, uint32_t count);
	};
}