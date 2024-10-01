#pragma once

#include <functional>

#include "glash/Core/Core.hpp"
#include "glash/Renderer/Buffer.hpp"

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
		using CreateIndexBufferCallback = std::function<Ref<IndexBuffer> (const uint32_t*, uint32_t size)>;

		Renderer(RendererAPI api);
		static const Renderer& Get();

	public:
		CreateVertexBufferCallback CreateVertexBufferFn;
		CreateIndexBufferCallback CreateIndexBufferFn;

	private:
		static Renderer* s_Instance;
	};
}