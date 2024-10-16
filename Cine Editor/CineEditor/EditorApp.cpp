#include "glash/glash_pch.hpp"
#include "glash/Core/EntryPoint.hpp"

#include "EditorLayer.hpp"

namespace Cine
{
	

	class CineEditor : public Application
	{
	public:
		CineEditor() : Application("Cine Editor") { }
	};

	Application* Cine::CreateApplication()
	{
		auto app = new CineEditor();
		app->PushLayer(new EditorLayer());
		return app;
	}
}


