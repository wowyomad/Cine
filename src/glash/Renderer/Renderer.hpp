#pragma once

#include <functional>

#include "glash/Core/Core.hpp"
#include "glash/Renderer/Buffer.hpp"
#include "glash/Renderer/VertexArray.hpp"

namespace glash
{
	enum class RendererAPI
	{
		None = 0,
		OpenGL,
	};

	class Renderer
	{


	public:
		using CreateVertexBufferCallback = std::function<Ref<VertexBuffer> (const float*, uint32_t size)>;
		using CreateVertexBufferEmptyCallback = std::function<Ref<VertexBuffer>(uint32_t size)>;
		using CreateIndexBufferCallback = std::function<Ref<IndexBuffer> (const uint32_t*, uint32_t size)>;
		using CreateVertexArrayCallback = std::function<Ref<VertexArray>()>;

		Renderer(RendererAPI api);
		static const Renderer& Get();

	public:
		CreateVertexBufferCallback CreateVertexBuffer;
		CreateVertexBufferEmptyCallback CreateVertexBufferEmpty;
		CreateIndexBufferCallback CreateIndexBuffer;
		CreateVertexArrayCallback CreateVertexArray;

	private:
		static Renderer* s_Instance;
	};
}