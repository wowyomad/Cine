#include "glash/glash_pch.hpp"
#include "glash/Platform/OpenGL/OpenGLVertexArray.hpp"

namespace glash
{
	static GLenum ShaderDataTypeToOpenGLEnumType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return	GL_FLOAT;
		case ShaderDataType::Float2:   return	GL_FLOAT;
		case ShaderDataType::Float3:   return	GL_FLOAT;
		case ShaderDataType::Float4:   return	GL_FLOAT;
		case ShaderDataType::Mat4:     return	GL_FLOAT;
		case ShaderDataType::Int:      return	GL_INT;
		case ShaderDataType::Int2:     return	GL_INT;
		case ShaderDataType::Int3:     return	GL_INT;
		case ShaderDataType::Int4:     return	GL_INT;
		case ShaderDataType::Bool:     return	GL_BOOL;
		}
		GLASH_CORE_ASSERT(false, "How did you get here?");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
		:m_VertexBufferIndex(0)
	{
		glCreateVertexArrays(1, &m_RendererID);
	}
	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}
	void OpenGLVertexArray::Bind()
	{
		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::Unbind()
	{
		glBindVertexArray(0);
	}
	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();

		GLASH_CORE_ASSERT(layout.Elements().size() > 0, "No layout in Vertex Buffer");
		
		for (auto& element : layout)
		{
			glEnableVertexAttribArray(m_VertexBufferIndex);
			glVertexAttribPointer(m_VertexBufferIndex, element.GetCount(), ShaderDataTypeToOpenGLEnumType(element.Type), element.Normalized, layout.Stride(), reinterpret_cast<const void*>(element.Offset));
			m_VertexBufferIndex++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}
	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}