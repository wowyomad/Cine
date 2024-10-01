#pragma once

#include "glash/Core/Core.hpp"

namespace glash
{
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static constexpr uint32_t ShaderDataTypeSize(ShaderDataType type)
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
	}

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void SetData(const void* data, uint32_t size) = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(const float* vertices, uint32_t count);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(const uint32_t* indices, uint32_t size);
	};
}